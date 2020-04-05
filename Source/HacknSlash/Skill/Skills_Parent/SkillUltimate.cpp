// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUltimate.h"
#include "Engine/World.h"
#include "Skill/SkillManagerComponent.h"
#include "Interfaces/Damagable.h"
#include "Kismet/GameplayStatics.h"
#include "HacknSlashPlayerController.h"
#include "HacknSlashCharacter.h"
#include "Components/SkeletalMeshComponent.h"

USkillUltimate::USkillUltimate()
{
	ExecuteTimeTimer = 0.0f;
}

void USkillUltimate::Execute()
{
	Super::Execute();
	hasFiredFirstBlast = false;
}

void USkillUltimate::LateExecute()
{
	ASkillCollisionDetection* holder = GetWorld()->SpawnActorDeferred<ASkillCollisionDetection>(detectionMesh, SkillMgrOwner->GetOwner()->GetTransform());
	if (holder != nullptr)
	{
		holder->skillOwner = this;
		UGameplayStatics::FinishSpawningActor(holder, SkillMgrOwner->GetOwner()->GetTransform());
		holder->SetActorLocation(Cref->GetMesh()->GetSocketLocation("weapon_Socket_FX"));
	}
	if (!hasFiredFirstBlast)
	{
		hasFiredFirstBlast = true;
	}
}

bool USkillUltimate::RequestExecution()
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

void USkillUltimate::EndExecution()
{
	ExecuteTimeTimer = 0.0f;
	hasFiredFirstBlast = false;
	Super::EndExecution();
}

void USkillUltimate::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult, AActor * OwnerActor)
{
	IDamagable* enemyHolder = Cast<IDamagable>(OtherActor);

	if (enemyHolder != nullptr)
	{
		if (hasFiredFirstBlast)
		{
			ACharacter *AI = Cast<ACharacter>(OtherActor);
			if (AI != nullptr)
			{
				UGameplayStatics::SpawnEmitterAttached(OnImpactFX, AI->GetMesh(), "pelvisSocket");
			}
		}
		float distance = FVector::Distance(OtherActor->GetActorLocation(), OwnerActor->GetActorLocation());
		float Ratio = 1.0f - (distance / range);
		Ratio = FMath::Clamp<float>(Ratio, 0.1f, 1.0f);
		enemyHolder->TakeDamage(hasFiredFirstBlast ? damageSecondBlast * Ratio : damageFirstBlast * Ratio, Cref);
	}
}

void USkillUltimate::TickSkill(float _deltaTime)
{
	ExecuteTimeTimer += _deltaTime;

	if (ExecuteTimeTimer >= ExecuteTime)
	{
		EndExecution();
	}
}

void USkillUltimate::InitSkill()
{
	Super::InitSkill();
	Cref = Cast<AHacknSlashCharacter>(SkillMgrOwner->GetOwner());
	PCref = Cast<AHacknSlashPlayerController>(Cref->GetController());
}
