// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillCollisionDetection.h"
#include "AutoAttack_AI.generated.h"

/**
 * 
 */

class AAgent_Base;

UCLASS(Abstract)
class HACKNSLASH_API UAutoAttack_AI : public USkillBase
{
	GENERATED_BODY()
	UAutoAttack_AI();

	UPROPERTY(EditAnywhere)
		float ExecuteTime;

	float ExecuteTimeTimer;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASkillCollisionDetection> detectionMesh;

	virtual void Execute() override;

public:
	UFUNCTION(BlueprintCallable)
	void LateExecute();


	UPROPERTY(BlueprintReadOnly)
	AAgent_Base* AI_CharacterRef;

	UPROPERTY(EditAnywhere)
	UParticleSystem* OnImpactFX;

private:
	virtual bool RequestExecution() override;

	virtual void EndExecution() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult, AActor* OwnerActor = NULL);

	virtual void TickSkill(float _deltaTime) override;

	virtual void InitSkill() override;

	AActor* playerRef;

};
