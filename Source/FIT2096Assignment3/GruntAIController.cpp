// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAIController.h"

AGruntAIController::AGruntAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGruntAIController::OnSensesUpdated);

	TargetPlayer = nullptr;
	StartLocationSet = false;
}

void AGruntAIController::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
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
}

void AGruntAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedPawn = InPawn;
	/*
	if (InPawn != nullptr)
	{
		BlackboardComponent->SetValueAsVector("OriginalLocation", InPawn->GetActorLocation());
	}
	*/
}

void AGruntAIController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedPawn = nullptr;
}

void AGruntAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!StartLocationSet)
	{
		BlackboardComponent->SetValueAsVector("OriginalLocation", PossessedPawn->GetActorLocation());
		StartLocationSet = true;
	}

	if (TargetPlayer) {
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AGruntAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}

	return FRotator::ZeroRotator;
}

void AGruntAIController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AGruntAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);

	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled()) {
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Actor Location"));
			TargetPlayer = TemporaryPawn;
			BlackboardComponent->SetValueAsBool("ChasePlayer", true);
			BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("ChasePlayer");
		}
	}
}
