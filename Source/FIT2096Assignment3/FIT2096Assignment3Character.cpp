// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT2096Assignment3Character.h"
#include "FIT2096Assignment3Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// AFIT2096Assignment3Character

AFIT2096Assignment3Character::AFIT2096Assignment3Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Set speeds
	WalkSpeedMax = 555.0f;
	SprintSpeedMax = 900.0f;

	// Set character initial speeds
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedMax;

	// Default offset from the character location for grapple projectiles to spawn
	GrappleOffset = FVector(100.0f, 0.0f, 0.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Assign Mesh for GrappleProjectile
	static ConstructorHelpers::FObjectFinder<UBlueprint> GrappleProjectileBlueprint(TEXT("Blueprint'/Game/GrappleHook/BP_GrappleProjectile.BP_GrappleProjectile'"));
	if (GrappleProjectileBlueprint.Object) {
		GrappleProjectileClass = GrappleProjectileBlueprint.Object->GeneratedClass;
	}

}

void AFIT2096Assignment3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////// Input

void AFIT2096Assignment3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind sprint events
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFIT2096Assignment3Character::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFIT2096Assignment3Character::StopSprint);

	// Bind grapple events
		// Bind sprint events
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &AFIT2096Assignment3Character::StartGrapple);
	PlayerInputComponent->BindAction("Grapple", IE_Released, this, &AFIT2096Assignment3Character::StopGrapple);

	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFIT2096Assignment3Character::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFIT2096Assignment3Character::StopCrouch);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFIT2096Assignment3Character::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFIT2096Assignment3Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFIT2096Assignment3Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFIT2096Assignment3Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFIT2096Assignment3Character::LookUpAtRate);
}

void AFIT2096Assignment3Character::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void AFIT2096Assignment3Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFIT2096Assignment3Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AFIT2096Assignment3Character::MoveForward(float Value)
{
	// FString MaxAccel = FString::SanitizeFloat(GetCharacterMovement()->GetMaxAcceleration());
	// GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::White, MaxAccel);
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFIT2096Assignment3Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFIT2096Assignment3Character::StartSprint()
{
	// GetCharacterMovement()->MaxAcceleration = 10;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeedMax;
}

void AFIT2096Assignment3Character::StopSprint()
{
	// GetCharacterMovement()->MaxAcceleration = 100;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedMax;
}

void AFIT2096Assignment3Character::StartCrouch()
{
	Crouch();
}

void AFIT2096Assignment3Character::StopCrouch()
{
	UnCrouch();
}

void AFIT2096Assignment3Character::StartGrapple()
{

	UWorld* World = GetWorld();

	// Run code if only World Exists
	if (World)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// GrappleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final spawning position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GrappleOffset);
		
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams = FActorSpawnParameters();
		ActorSpawnParams.bNoFail = true;
		ActorSpawnParams.Owner = this;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		
		// Spawn the projectile at the correct position
		SpawnedGrapple = Cast<AGrappleProjectile>(World->SpawnActor<AGrappleProjectile>(GrappleProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams));

		// Rotate SpawnedGrapple to match Camera
		SpawnedGrapple->FireInDirection(SpawnRotation.Vector());
	}
}

void AFIT2096Assignment3Character::StopGrapple()
{
	// Check if the GrappleHook has already been destroyed
	if (SpawnedGrapple != nullptr && this != nullptr)
	{
		SpawnedGrapple->Destroy();
	}
}

void AFIT2096Assignment3Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFIT2096Assignment3Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AFIT2096Assignment3Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFIT2096Assignment3Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFIT2096Assignment3Character::EndTouch);

		return true;
	}
	
	return false;
}
