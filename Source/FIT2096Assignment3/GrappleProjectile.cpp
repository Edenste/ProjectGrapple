	// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleProjectile.h"

// Sets default values
AGrappleProjectile::AGrappleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialise Projectile Properties
	ProjectileSpeed = 2250;

	// Setup Mesh Component
	GrappleProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grapple Projectile Mesh"));
	RootComponent = GrappleProjectileMesh;

	// Generate Projectile Movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
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
	
	
}

// Called every frame
void AGrappleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AGrappleProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AGrappleProjectile::OnGrappleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Prevent collision detection with Parent and ensure it is actually colliding with something
	if ((OtherActor != this && OtherActor != Owner && OtherActor != nullptr))
	{
		
		// Disable and collision and attach Grapple to new Actor
		SetActorEnableCollision(false);
		ProjectileMovementComponent->InitialSpeed = 0;
		ProjectileMovementComponent->MaxSpeed = 0;
		AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	}
}
