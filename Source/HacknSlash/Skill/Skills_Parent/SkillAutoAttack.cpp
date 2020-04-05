// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAutoAttack.h"
#include "Engine/World.h"
#include "Skill/SkillManagerComponent.h"
#include "Interfaces/Damagable.h"
#include "Kismet/GameplayStatics.h"
#include "HacknSlashPlayerController.h"
#include "HacknSlashCharacter.h"

USkillAutoAttack::USkillAutoAttack()
{
	ExecuteTimeTimer = 0.0f;
}

void USkillAutoAttack::Execute()
{
	Super::Execute();
}

void USkillAutoAttack::LateExecute()
{
	ASkillCollisionDetection* holder = GetWorld()->SpawnActorDeferred<ASkillCollisionDetection>(detectionMesh, SkillMgrOwner->GetOwner()->GetTransform());
	if (holder != nullptr)
	{
		holder->skillOwner = this;
		UGameplayStatics::FinishSpawningActor(holder, SkillMgrOwner->GetOwner()->GetTransform());
		holder->SetActorLocation(SkillMgrOwner->GetOwner()->GetActorLocation() + SkillMgrOwner->GetOwner()->GetActorForwardVector() * 175.0f);
	}
}

bool USkillAutoAttack::RequestExecution()
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

void USkillAutoAttack::EndExecution()
{
	ExecuteTimeTimer = 0.0f;
	Super::EndExecution();
}

void USkillAutoAttack::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult, AActor* OwnerActor)
{	
	if (OtherActor != PCref->GetTargetToAutoAttack())
	{
		return;
	}

	IDamagable* enemyHolder = Cast<IDamagable>(OtherActor);

	if (enemyHolder != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnImpactFX, OtherActor->GetActorLocation());
		enemyHolder->TakeDamage(damages, Cref);
	}
}

void USkillAutoAttack::TickSkill(float _deltaTime)
{
	ExecuteTimeTimer += _deltaTime;

	if (ExecuteTimeTimer >= ExecuteTime)
	{
		EndExecution();
	}
}

void USkillAutoAttack::InitSkill()
{
	Super::InitSkill();
	Cref = Cast<AHacknSlashCharacter>(SkillMgrOwner->GetOwner());
	PCref = Cast<AHacknSlashPlayerController>(Cref->GetController());
}

