// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrappleProjectile.h"
#include "NiagaraSystem.h"
// #include "CableComponent.h"
#include "FIT2096Assignment3Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AFIT2096Assignment3Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFIT2096Assignment3Character();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float WalkSpeedMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float SprintSpeedMax;

	/** Grapple's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FVector GrappleOffset;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AGrappleProjectile> GrappleProjectileClass;

	/*
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrappleProjectile> GrappleCableClass;
	*/

	/** Used to keep track of grapple projectiles spawned by the player in the event they need to be destroyed by the Character*/
	UPROPERTY()
	AGrappleProjectile* SpawnedGrapple;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* SB_DoubleJump;

	UPROPERTY(EditAnywhere, Category = "Niagara")
		UNiagaraSystem* NS_DoubleJump;

protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/** Handles initiating sprinting movement */
	void StartSprint();

	/** Handles stopping sprinting movement */
	void StopSprint();

	/** Handles initiating a crouch or a slide if sprint button is being pressed */
	void StartCrouch();

	/** Handles stopping a crouch or a slide */
	void StopCrouch();

	/** Internal handling of the jump function to allow for the creation of SFX and Sounds **/
	void StartJump();
	
	/** Handles firing a grapple hook */
	void StartGrapple();

	/** Handles stopping and destroying a grapple hook */
	void StopGrapple();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

