// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceedToAttack.h"
#include "AIController.h"
#include "AI/Agent_Base.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Skill/SkillManagerComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HacknSlashCharacter.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UProceedToAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if (playerRef == nullptr)
	{
		playerRef = Cast<AHacknSlashCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player"));
	}
	
	if (FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), playerRef->GetActorLocation()) > 200.0f)
	{
		return EBTNodeResult::Failed;
	}
	else if (playerRef->GetHP() == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(("Player"), NULL);
		return EBTNodeResult::Failed;
	}
	OwnerComp.GetAIOwner()->GetCharacter()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), playerRef->GetActorLocation()));
	
	Cast<AAgent_Base>(OwnerComp.GetAIOwner()->GetCharacter())->SkillManagerAI->OnSkillRequested(AAgent_Base::IDSKILLSAI::autoAttack);

	return EBTNodeResult::InProgress;
}

void UProceedToAttack::InitializeMemory(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	//End this Task on message sent by blueprint at the end of play montage playing autoattack animation
	OwnerComp.RegisterMessageObserver(this, "AnimDone");
}

void UProceedToAttack::OnMessage(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	if(Message == FName("AnimDone"))
	{
		OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
	}
}