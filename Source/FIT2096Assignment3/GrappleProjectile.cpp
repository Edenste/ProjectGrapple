	// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleProjectile.h"

// Sets default values
AGrappleProjectile::AGrappleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Mesh Component
	GrappleProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grapple Projectile Mesh"));
	RootComponent = GrappleProjectileMesh;

	// Initialise Projectile Properties
	Parent = nullptr;
	ProjectileSpeed = 600;
}

// Called when the game starts or when spawned
void AGrappleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GrappleProjectileMesh->OnComponentHit.AddDynamic(this, &AGrappleProjectile::OnGrappleHit);
}

// Called every frame
void AGrappleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Compute movement vector for GrappleProjectile
	FVector CurrentLocation = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector Movement = (Forward * ProjectileSpeed * DeltaTime);
	FVector NewLocation = CurrentLocation + Movement;

	// Move GrappleProjectile to new location
	SetActorLocation(NewLocation);
}

void AGrappleProjectile::OnGrappleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Prevent 
}