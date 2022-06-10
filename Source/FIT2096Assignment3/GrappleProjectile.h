// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "GrappleProjectile.generated.h"

UCLASS()
class FIT2096ASSIGNMENT3_API AGrappleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleProjectile();

public:
	// Used to check if grapple has hooked to anything and Owner should start being reeled towards it
	UPROPERTY()
		bool Deployed;

	// Used to launch player through LaunchCharacter by casting Owner APawn* to ACharacter*
	UPROPERTY()
		ACharacter* Player;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* GrappleProjectileMesh;

	// Used to govern how fast the initial GrappleProjectile moves
	UPROPERTY(EditAnywhere)
		float ProjectileSpeed;

	// Used to govern how fast the player is reeled in towards the grapple hook
	UPROPERTY()
		float GrappleLaunchSpeed;

	// If Character is too close to grapple then used to break grapple
	UPROPERTY()
		float MinCharacterDistance;

	// Governs Projectile Movement
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* SB_GrappleDeploy;

	UPROPERTY(EditAnywhere, Category = "Niagara")
		UNiagaraSystem* NS_GrappleDeploy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
		void OnGrappleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	// Function calculates the vector between the Character and GrappleProjectile
	FVector CharacterToGrapple();

	// Function used to calculate how much player is launched by and in which direction
	// by finding the inbetween vector of the camera forward and the difference between
	// the player and the GrappleProjectile
	FVector GetLaunchVector();

	// Function to calculate the Grapple break conditions when deployed which are:
	// Break the Grapple if Player goes past the Grapple
	// Break the Grapple if Player is near the Grapple
	bool GrappleBreakConditions();
};
