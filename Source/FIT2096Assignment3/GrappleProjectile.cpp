	// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "GrappleProjectile.h"

// Sets default values
AGrappleProjectile::AGrappleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialise Projectile Properties
	ProjectileSpeed = 3500;
	Deployed = false;
	GrappleLaunchSpeed = 12000;
	MinCharacterDistance = 200;

	// Setup Mesh Component
	GrappleProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grapple Projectile Mesh"));
	RootComponent = GrappleProjectileMesh;

	// Generate Projectile Movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// Add collision handling
	GrappleProjectileMesh->OnComponentHit.AddDynamic(this, &AGrappleProjectile::OnGrappleHit);
}

// Called when the game starts or when spawned
void AGrappleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<ACharacter>(Owner);
}

// Called every frame
void AGrappleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Start grappling player towards grappling hook if it has been deployed
	if (Deployed && Owner != nullptr)
	{
		// Calculate if the grapple should be broken
		if (GrappleBreakConditions())
		{
			Destroy();
		}

		// Find LaunchVector and multiply by DeltaTime
		FVector LaunchVector = GetLaunchVector() * DeltaTime;

		Player->LaunchCharacter(LaunchVector, false, false);
	}
}

void AGrappleProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AGrappleProjectile::OnGrappleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Prevent collision detection with Parent and ensure it is actually colliding with something
	if ((OtherActor != this && OtherActor != Owner && OtherActor != nullptr && Deployed != true))
	{
		
		// Disable and collision and attach Grapple to new Actor
		SetActorEnableCollision(false);
		ProjectileMovementComponent->InitialSpeed = 0;
		ProjectileMovementComponent->MaxSpeed = 0;
		AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
		Deployed = true;

		// Spawn GrappleDeploy SFX
		if (SB_GrappleDeploy)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_GrappleDeploy, OtherActor->GetActorLocation());
		}
	}
}

FVector AGrappleProjectile::CharacterToGrapple()
{
	// Calculate the vector between the player and the grappling hook
	FVector PlayerLocation = Player->GetActorLocation();
	FVector GrappleLocation = GetActorLocation();
	return GrappleLocation - PlayerLocation;
}

FVector AGrappleProjectile::GetLaunchVector()
{
	if (Owner != nullptr && Player != nullptr)
	{
		// Calculate the vectors between the player and the grappling hook
		FVector DestinationVector = CharacterToGrapple();
		DestinationVector.Normalize();

		// Calculate the forward vector of the camera
		APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
		FVector CameraForward = PlayerController->PlayerCameraManager->GetActorForwardVector();

		// Add together the two Vectors of Launch and Camera to find the inbetween vector
		FVector LaunchVector = DestinationVector + CameraForward;
		LaunchVector.Normalize();
		LaunchVector *= GrappleLaunchSpeed;

		return LaunchVector;
	}
	else
	{
		return FVector::Zero();
	}
}

bool AGrappleProjectile::GrappleBreakConditions()
{
	// Only go through break conditions if grapple is deployed
	if (Deployed)
	{
		// Initialise Vectors
		FVector DifferenceVector = CharacterToGrapple();
		DifferenceVector.Normalize();
		FVector GrappleForward = GetActorForwardVector();
		// GrappleLocation.Normalize();
		float DifferenceDistance = CharacterToGrapple().Size();

		// Break the Grapple if Player goes past the Grapple
		if (FVector::DotProduct(DifferenceVector, GrappleForward) < 0)
		{
			return true;
		}
		// Break the Grapple if Player is too near the Grapple
		else if (DifferenceDistance < MinCharacterDistance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}