// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAIController.h"

ATurretAIController::ATurretAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = SightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ATurretAIController::OnSensesUpdated);

	TargetPlayer = nullptr;
	StartLocationSet = false;
}

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIBlackboard)
	{
		return;
	}
	if (!ensure(BehaviorTree))
	{
		return;
	}

	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	BlackboardComponent->SetValueAsBool("CanFire", true);
}

void ATurretAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedPawn = InPawn;
}

void ATurretAIController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedPawn = nullptr;
}

void ATurretAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FRotator ATurretAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}

	return FRotator::ZeroRotator;
}

void ATurretAIController::Shoot()
{
	if (TargetPlayer != nullptr)
	{
		if (ProjectileClass)
		{
			// Calculate the vector between the shooter and target
			FVector ShooterOffset = FVector(35, 0, 200);
			FVector ShooterLocation = PossessedPawn->GetActorLocation();
			FVector StartPosition = ShooterLocation + ShooterOffset;

			FVector TargetLocation = TargetPlayer->GetActorLocation();
			FVector DifferenceVector = TargetLocation - StartPosition;
			DifferenceVector.Normalize();
			FRotator StartRotation = DifferenceVector.Rotation();

			// Spawn Projectile
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams = FActorSpawnParameters();
			ActorSpawnParams.bNoFail = true;
			ActorSpawnParams.Owner = this;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the correct position
			Cast<AFIT2096Assignment3Projectile>(GetWorld()->SpawnActor(ProjectileClass, &StartPosition, &StartRotation, ActorSpawnParams));

			// Setup timer to refresh Turret's ability to fire
			BlackboardComponent->SetValueAsBool("CanFire", false);
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ATurretAIController::OnTimedReload, FireCooldown, true);
		}
	}

}

void ATurretAIController::OnTimedReload()
{
	BlackboardComponent->SetValueAsBool("CanFire", true);

	// Clear timer to reload
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void ATurretAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);

	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled()) {
		if (Stimulus.WasSuccessfullySensed())
		{
			TargetPlayer = TemporaryPawn;
			BlackboardComponent->SetValueAsBool("PlayerTargeted", true);
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("PlayerTargeted");
		}
	}
}
