// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damagable.h"
#include "AI/Widgets/AI_Widget.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Agent_Base.generated.h"


class UAIPerceptionComponent;
class USkillManagerComponent;

UCLASS()
class HACKNSLASH_API AAgent_Base : public ACharacter, public IDamagable
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AAgent_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* widgetComponentContainer;

	UAI_Widget* widgetInfoDisplay;

	UBlackboardComponent* m_BC_Attached;

	UFUNCTION()
	void HandleAIPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

public:	

	enum IDSKILLSAI
	{
		autoAttack,
		countSkill
	};

	UPROPERTY(EditAnywhere)
	UParticleSystem* SpawnFX;

	UPROPERTY(EditAnywhere)
	uint32 DefineHP;

	virtual void TakeDamage(uint32 _damageValue, AActor* _damageMaker) override;

	void GetDataToBlackBoard(UBlackboardComponent* _pbbData);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere)
	class USkillManagerComponent* SkillManagerAI;

private:
	bool hasTarget;

	//Avoid repeting death sequence when multiple attacks are being sent to this target
	bool isAlreadyDead;

};
