// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntCharacter.h"

// Sets default values
AGruntCharacter::AGruntCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set movement variables
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 5, 0);
}

// Called when the game starts or when spawned
void AGruntCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGruntCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGruntCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

