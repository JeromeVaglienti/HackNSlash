// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillFire.h"
#include "Skill/SkillManagerComponent.h"
#include "Interfaces/Damagable.h"
#include "HacknSlashPlayerController.h"
#include "HacknSlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

USkillFire::USkillFire()
{
	ExecuteTimeTimer = 0.0f;
}


void USkillFire::Execute()
{
	shotsCount = 0;

	for (int i = 0; i < shotsCountTotal; i++)
	{
		projectileArray.Add(GetWorld()->SpawnActorDeferred<ASkillLinearProjectile>(projectileMesh, SkillMgrOwner->GetOwner()->GetTransform()));
	}

	Super::Execute();
}

void USkillFire::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult, AActor* OwnerActor)
{
	IDamagable* enemyHolder = Cast<IDamagable>(OtherActor);

	if (enemyHolder != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnImpactFX, OwnerActor->GetActorLocation());
		enemyHolder->TakeDamage(damages, Cref);
	}
}

void USkillFire::LateExecute()
{
	if (shotsCount < shotsCountTotal)
	{	
		ASkillLinearProjectile* holder = projectileArray[shotsCount];

		holder->Init(this, Cref->GetActorForwardVector(), projectileSpeed, range);
		UGameplayStatics::FinishSpawningActor(holder, SkillMgrOwner->GetOwner()->GetTransform());
		holder->SetActorLocation(Cref->GetMesh()->GetSocketLocation("weapon_Socket_FX") + SkillMgrOwner->GetOwner()->GetActorForwardVector() * 35.0f);
		shotsCount++;
	}
}

bool USkillFire::RequestExecution()
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

void USkillFire::EndExecution()
{
	ExecuteTimeTimer = 0.0f;
	shotsCount = 0;
	projectileArray.Empty();
	Super::EndExecution();
}

void USkillFire::TickSkill(float _deltaTime)
{
	ExecuteTimeTimer += _deltaTime;

	if (ExecuteTimeTimer >= ExecuteTime)
	{
		EndExecution();
	}
}

void USkillFire::InitSkill()
{
	Super::InitSkill();
	Cref = Cast<AHacknSlashCharacter>(SkillMgrOwner->GetOwner());
	PCref = Cast<AHacknSlashPlayerController>(Cref->GetController());
}
