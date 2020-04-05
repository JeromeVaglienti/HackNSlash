// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Base_AI_Controller.generated.h"

/**
 * 
 */

class AHacknSlashPlayerController;

UCLASS()
class HACKNSLASH_API ABase_AI_Controller : public AAIController
{
	GENERATED_BODY()
private:

	ABase_AI_Controller();

	int RangeForAttackingPosessedPawn;

	virtual void OnPossess(APawn* InPawn) override;
	UBlackboardComponent* m_pBBComponent;

	virtual void BeginPlay() override;

	bool bIsMouseOverred;

	AHacknSlashPlayerController* m_pPlayerController;

	UFUNCTION()
	void ClickedEventBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void ClickedEventEndMouseOver(UPrimitiveComponent* TouchedComponent);

public:
	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackBoardUsed;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeUsed;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	void ProceedEventDeath();
};
