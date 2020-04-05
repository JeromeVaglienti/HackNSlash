// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillLinearProjectile.h"
#include "SkillFire.generated.h"

/**
 * 
 */

class AHacknSlashPlayerController;
class AHacknSlashCharacter;

UCLASS(Abstract)
class HACKNSLASH_API USkillFire : public USkillBase
{
	GENERATED_BODY()
	USkillFire();

	UPROPERTY(EditAnywhere)
	float ExecuteTime;

	float ExecuteTimeTimer;

	TArray<ASkillLinearProjectile*> projectileArray;

	virtual void Execute() override;

	int shotsCount;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASkillLinearProjectile> projectileMesh;


	UPROPERTY(BlueprintReadOnly)
	AHacknSlashCharacter* Cref;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult,  AActor* OwnerActor = NULL) override;

	UPROPERTY(EditAnywhere)
	float projectileSpeed;

	UPROPERTY(EditAnywhere)
	float range;

	UPROPERTY(EditAnywhere)
	int shotsCountTotal;

	//Allow to execute logics of the skills that needs to be done during the skill execution
	//like Check Collision when the animation realy "hits"
	//Most likely called in BP on a notify task of the montage being played
	UFUNCTION(BlueprintCallable)
	void LateExecute();

	UPROPERTY(EditAnywhere)
	UParticleSystem* OnImpactFX;
	
private:
	virtual bool RequestExecution() override;

	virtual void EndExecution() override;

	virtual void TickSkill(float _deltaTime) override;

	virtual void InitSkill() override;

	AHacknSlashPlayerController* PCref;

};
