// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAttack_AI.h"
#include "Engine/World.h"
#include "Skill/SkillManagerComponent.h"
#include "Interfaces/Damagable.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Agent_Base.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UAutoAttack_AI::UAutoAttack_AI()
{
	ExecuteTimeTimer = 0.0f;
}

void UAutoAttack_AI::Execute()
{
	Super::Execute();
}

void UAutoAttack_AI::LateExecute()
{
	ASkillCollisionDetection* holder = GetWorld()->SpawnActorDeferred<ASkillCollisionDetection>(detectionMesh, SkillMgrOwner->GetOwner()->GetTransform());
	if (holder != nullptr)
	{
		holder->skillOwner = this;
		UGameplayStatics::FinishSpawningActor(holder, SkillMgrOwner->GetOwner()->GetTransform());
		holder->SetActorLocation(SkillMgrOwner->GetOwner()->GetActorLocation() + SkillMgrOwner->GetOwner()->GetActorForwardVector() * 175.0f);
	}
}

bool UAutoAttack_AI::RequestExecution()
{
	if (ExecuteTimeTimer == 0.0f)
	{
		Execute();
		return true;
	}
	else
	{
		return false;
	}
}

void UAutoAttack_AI::EndExecution()
{
	ExecuteTimeTimer = 0.0f;
	Super::EndExecution();
}

void UAutoAttack_AI::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult, AActor* OwnerActor)
{
	if (OtherActor != playerRef)
	{
		return;
	}

	IDamagable* enemyHolder = Cast<IDamagable>(OtherActor);

	if (enemyHolder != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnImpactFX, OtherActor->GetActorLocation());
		enemyHolder->TakeDamage(damages, AI_CharacterRef);
	}
}

void UAutoAttack_AI::TickSkill(float _deltaTime)
{
	ExecuteTimeTimer += _deltaTime;

	if (ExecuteTimeTimer >= ExecuteTime)
	{
		EndExecution();
	}
}

void UAutoAttack_AI::InitSkill()
{
	Super::InitSkill();

	FTimerHandle skillHandler;

	FTimerDelegate skillDelegate = FTimerDelegate::CreateLambda([=]()
	{
		AI_CharacterRef = Cast<AAgent_Base>(SkillMgrOwner->GetOwner());
		playerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	});

	GetWorld()->GetTimerManager().SetTimer(skillHandler, skillDelegate, 0.15f, false);
}

