// Fill out your copyright notice in the Description page of Project Settings.


#include "IsAtHome.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UIsAtHome::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsAtHome");
}
