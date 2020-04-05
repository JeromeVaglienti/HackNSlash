// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AI_Controller.h"
#include "HacknSlashPlayerController.h"
#include "HacknSlashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "AI/Agent_Base.h"
#include "BrainComponent.h"
#include "Kismet/GameplayStatics.h"


ABase_AI_Controller::ABase_AI_Controller()
{
	SetGenericTeamId(FGenericTeamId(5));
	bIsMouseOverred = false;
}


void ABase_AI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BlackBoardUsed != nullptr)
	{
		UseBlackboard(BlackBoardUsed, m_pBBComponent);
	}


	if (BehaviorTreeUsed != nullptr)
	{
		RunBehaviorTree(BehaviorTreeUsed);
	}

	AAgent_Base* AAIC = Cast<AAgent_Base>(InPawn);

	if (AAIC != nullptr && m_pBBComponent != nullptr)
	{
		AAIC->GetDataToBlackBoard(m_pBBComponent);
	}

	//Bind mouse to target for autoAttacking
	(Cast<ACharacter>(GetPawn()))->GetCapsuleComponent()->OnBeginCursorOver.AddDynamic(this, &ABase_AI_Controller::ClickedEventBeginMouseOver);

	(Cast<ACharacter>(GetPawn()))->GetCapsuleComponent()->OnEndCursorOver.AddDynamic(this, &ABase_AI_Controller::ClickedEventEndMouseOver);

	RangeForAttackingPosessedPawn = Cast<AAgent_Base>(GetPawn())->GetRange();

	if(AAIC->SpawnFX != NULL)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AAIC->SpawnFX, AAIC->GetActorLocation());
	}
}


void ABase_AI_Controller::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayerController = Cast<AHacknSlashPlayerController>(GetWorld()->GetFirstPlayerController());
}

void ABase_AI_Controller::ClickedEventBeginMouseOver(UPrimitiveComponent * TouchedComponent)
{
	m_pPlayerController->EnnemyMouseOverHandle(true, GetPawn(), RangeForAttackingPosessedPawn);
	bIsMouseOverred = true;
}

void ABase_AI_Controller::ClickedEventEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	m_pPlayerController->EnnemyMouseOverHandle(false, GetPawn());
	bIsMouseOverred = false;
}

ETeamAttitude::Type ABase_AI_Controller::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn * OtherPawn = Cast<APawn>(&Other)) 
	{
		if (const IGenericTeamAgentInterface * TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID == 10) 
			{
				return ETeamAttitude::Friendly;
			}
			else {
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

void ABase_AI_Controller::ProceedEventDeath()
{
	(Cast<ACharacter>(GetPawn()))->GetCapsuleComponent()->OnBeginCursorOver.RemoveDynamic(this, &ABase_AI_Controller::ClickedEventBeginMouseOver);

	(Cast<ACharacter>(GetPawn()))->GetCapsuleComponent()->OnEndCursorOver.RemoveDynamic(this, &ABase_AI_Controller::ClickedEventEndMouseOver);
	if (bIsMouseOverred)
	{
		m_pPlayerController->EnnemyMouseOverHandle(false, GetPawn());
	}
	GetBrainComponent()->StopLogic("IsDead");
	m_pPlayerController->TargetHasDied(GetPawn());
	Destroy();
}
