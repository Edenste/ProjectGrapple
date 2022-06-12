// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT2096Assignment3Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFIT2096Assignment3Projectile::AFIT2096Assignment3Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFIT2096Assignment3Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFIT2096Assignment3Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics 
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		// Add Impulse if a Component
		if ((OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

			Destroy();
		}
		// Add Launch Vector if Actor
		if (OtherActor != nullptr && OtherActor->CanBeDamaged())
		{
			
			ACharacter* Target = Cast<ACharacter>(OtherActor);

			// Calculate vector that OtherActor should be launched
			FVector TargetLocation = Target->GetActorLocation();
			FVector DifferenceVector = GetActorLocation() - Target->GetActorLocation();
			DifferenceVector.Normalize();
			FRotator Rotation = DifferenceVector.Rotation();
			Rotation = Rotation * -1; // Reverse the rotation so we fling the target in the opposite direction

			FVector LaunchVector = Rotation.RotateVector(FVector(1000, 0, 0));

			// Launch target and destroy projectile
			Target->LaunchCharacter(LaunchVector, false, false);
			Destroy();
		}
	}
}