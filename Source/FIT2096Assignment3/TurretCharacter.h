// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TurretCharacter.generated.h"

UCLASS()
class FIT2096ASSIGNMENT3_API ATurretCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurretCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Mesh Component Variables
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TurretBody;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TurretGun;

};
