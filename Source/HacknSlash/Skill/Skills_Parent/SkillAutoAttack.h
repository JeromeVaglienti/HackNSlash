// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillCollisionDetection.h"
#include "SkillAutoAttack.generated.h"

/**
 * 
 */
class AHacknSlashPlayerController;
class AHacknSlashCharacter;

UCLASS(Abstract)
class HACKNSLASH_API USkillAutoAttack : public USkillBase
{
	GENERATED_BODY()
	USkillAutoAttack();

	UPROPERTY(EditAnywhere)
	float ExecuteTime;

	float ExecuteTimeTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASkillCollisionDetection> detectionMesh;

	virtual void Execute() override;

public:
	UFUNCTION(BlueprintCallable)
	void LateExecute();

	UPROPERTY(EditAnywhere)
	UParticleSystem* OnImpactFX;


	UPROPERTY(BlueprintReadOnly)
	AHacknSlashCharacter* Cref;

private:
	virtual bool RequestExecution() override;

	virtual void EndExecution() override;

public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult, AActor* OwnerActor = NULL);

private:
	virtual void TickSkill(float _deltaTime) override;

	virtual void InitSkill() override;


	AHacknSlashPlayerController* PCref;
};
