// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerFound.h"
#include "BehaviorTree/BlackboardComponent.h"


bool UIsPlayerFound::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player") != NULL;
}
