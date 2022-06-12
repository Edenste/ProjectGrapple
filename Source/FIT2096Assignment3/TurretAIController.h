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
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIT2096ASSIGNMENT3_API ATurretAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATurretAIController();
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
		float SightRadius = 8000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightAge = 7;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float FieldOfView = 360;

	UPROPERTY(EditAnywhere, Category = "AI")
		float FireCooldown = 0.1f;		// Limits the Turret's capacity to fire

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

	APawn* TargetPlayer;
	bool StartLocationSet;
	APawn* PossessedPawn;

	FTimerHandle ReloadTimerHandle;
};
