// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class HACKNSLASH_API UMoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	UMoveToTarget();

	AActor* playerRef;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void hasArrivedHandle(FAIRequestID RequestID, const FPathFollowingResult& Result);

	bool hasArrived;

	float timer;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
