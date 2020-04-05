// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToTarget.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UMoveToTarget::UMoveToTarget()
{
	bNotifyTick = true;
	hasArrived = false;
	timer = 0.0f;
}

EBTNodeResult::Type UMoveToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if (playerRef == nullptr)
	{
		playerRef = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player"));
	}
	else if (FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), playerRef->GetActorLocation()) < 200.0f)
	{
		return EBTNodeResult::Succeeded;
	}

	EPathFollowingRequestResult::Type requestResult = OwnerComp.GetAIOwner()->MoveToActor((playerRef), 80.0f);

	//Bind delegate To avoid heavy calculation on nodeTick
	OwnerComp.GetAIOwner()->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UMoveToTarget::hasArrivedHandle);
	return EBTNodeResult::InProgress;
}

void UMoveToTarget::hasArrivedHandle(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	hasArrived = true;
}

void UMoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	timer += DeltaSeconds;

	//Avoid deadlock when sometimes the agent never reach it destination
	//Therefore the delegate is never broadcast and hasArrived never become true
	if (timer > 10.0f)
	{
		hasArrived = true;
		OwnerComp.GetAIOwner()->StopMovement();
		timer = 0.0f;
	}
	
	if (hasArrived)
	{
		if (FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), playerRef->GetActorLocation()) < 200.0f)
		{
			OwnerComp.GetAIOwner()->StopMovement();
			OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
		}
		else
		{
			//Is too far move again to the player
			OwnerComp.GetAIOwner()->MoveToActor((playerRef), 80.0f);
		}
		hasArrived = false;
	}
}
