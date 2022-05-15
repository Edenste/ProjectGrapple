// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleProjectile.generated.h"

UCLASS()
class FIT2096ASSIGNMENT3_API AGrappleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleProjectile();

public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* GrappleProjectileMesh;

	// Keeps track of which actor that spawned this grapple to know what to draw towards
	UPROPERTY(EditAnywhere)
		AActor* Parent;

	UPROPERTY(EditAnywhere)
		float ProjectileSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnGrappleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
