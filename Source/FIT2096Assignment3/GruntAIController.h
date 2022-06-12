// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FIT2096Assignment3Projectile.h"
#include "GruntAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIT2096ASSIGNMENT3_API AGruntAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Functions
	AGruntAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void GenerateNewRandomLocation();
	void Shoot();
	void OnTimedReload();

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

public:
	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightRadius = 5000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightAge = 3.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float LoseSightRadius = SightRadius + 3000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float FieldOfView = 90;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardData* AIBlackboard;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardComponent* BlackboardComponent;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFIT2096Assignment3Projectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		int ShootRange = 1000;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;
	bool StartLocationSet;
	APawn* PossessedPawn;

	UPROPERTY(EditAnywhere, Category = "AI")
		int FireCooldown = 3;		// Limits the Grunt's capacity to fire to once every 3 seconds

	FTimerHandle ReloadTimerHandle;
};
