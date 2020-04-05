// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */

class USkillManagerComponent;

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class HACKNSLASH_API USkillBase : public UObject
{
	GENERATED_BODY()
public:

	//Giving the option to send the owneractor if you want to spawn visual feedback on the owner actor and not on the other actor
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult, AActor* OwnerActor = NULL)
	{
		Event_OnOverlapBeginBP(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}

	UFUNCTION(BlueprintImplementableEvent)
		void Event_OnOverlapBeginBP(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
	{
		Event_OnOverlapEndBP(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
	}

	UFUNCTION(BlueprintImplementableEvent)
		void Event_OnOverlapEndBP(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	int32 range;
	float cooldown;
	float cooldownTimer;
	bool isExecuting;

	UPROPERTY(EditAnywhere)
	int32 damages;

	UPROPERTY(EditAnywhere)
	bool CanBeOverrideByOtherSkill;

	UPROPERTY(EditAnywhere)
	bool bSkillIsOneTickOnly;

	virtual void Execute();

	UFUNCTION(BlueprintImplementableEvent)
		void Event_ExecuteBP();

	virtual void EndExecution();

	UFUNCTION(BlueprintImplementableEvent)
		void Event_EndExecutionBP();
public:

	UPROPERTY(BlueprintReadOnly)
	USkillManagerComponent* SkillMgrOwner;
	FORCEINLINE bool IsSkillExecuting() { return isExecuting; }
	virtual bool RequestExecution();
	virtual void TickSkill(float _deltaTime);

	//Allow to init skill by adding usefull references like the real class of the owner/controller
	//Avoiding Casting each time the skill is being called
	virtual void InitSkill();

	virtual void OnDetectionNegative()
	{
		OnDetectionNegative_BP();
	}
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDetectionNegative_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void Event_TickSkillBP(float _deltaTime);
};
