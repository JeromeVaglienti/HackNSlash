// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToHome.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UMoveToHome::UMoveToHome()
{
	hasArrived = false;
	bNotifyTick = true;
}


EBTNodeResult::Type UMoveToHome::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerComp.GetAIOwner()->MoveToLocation(OwnerComp.GetBlackboardComponent()->GetValueAsVector("HomeLocation"), 50.0f);

	OwnerComp.GetAIOwner()->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UMoveToHome::hasArrivedHandle);

	hasArrived = false;
	return EBTNodeResult::InProgress;
}

void UMoveToHome::hasArrivedHandle(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	hasArrived = true;
}

void UMoveToHome::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	if (hasArrived)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAtHome"), true);
		OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
	}
}
