// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TurretAIController.h"
#include "TurretShootTask.generated.h"

/**
 * 
 */
UCLASS()
class FIT2096ASSIGNMENT3_API UTurretShootTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
