// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretCharacter.h"

// Sets default values
ATurretCharacter::ATurretCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Turret Components
	// Every Actor contains a root component. We initialize this as a scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	TurretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Body"));
	TurretBody->SetupAttachment(RootComponent);

	TurretGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Gun"));
	TurretGun->SetupAttachment(TurretBody);
}

// Called when the game starts or when spawned
void ATurretCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATurretCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

