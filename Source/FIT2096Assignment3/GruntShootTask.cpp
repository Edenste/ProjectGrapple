// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntShootTask.h"

EBTNodeResult::Type UGruntShootTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component)
	{
		return EBTNodeResult::Failed;
	}

	AGruntAIController* MyController = Cast<AGruntAIController>(Component->GetOwner());
	if (!MyController)
	{
		return EBTNodeResult::Failed;
	}

	MyController->Shoot();
	return EBTNodeResult::Succeeded;
}