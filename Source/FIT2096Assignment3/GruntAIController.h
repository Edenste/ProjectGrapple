// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void GenerateNewRandomLocation();

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightRadius = 500;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightAge = 3.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float LoseSightRadius = SightRadius + 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float FieldOfView = 60;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardData* AIBlackboard;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardComponent* BlackboardComponent;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;
};
