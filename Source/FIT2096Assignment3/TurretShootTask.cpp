// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretShootTask.h"

EBTNodeResult::Type UTurretShootTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component)
	{
		return EBTNodeResult::Failed;
	}

	ATurretAIController* MyController = Cast<ATurretAIController>(Component->GetOwner());
	if (!MyController)
	{
		return EBTNodeResult::Failed;
	}

	MyController->Shoot();
	return EBTNodeResult::Succeeded;
}