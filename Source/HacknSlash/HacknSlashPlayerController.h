// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HacknSlashPlayerController.generated.h"

UCLASS()
class AHacknSlashPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHacknSlashPlayerController();

	void EnnemyMouseOverHandle(bool isOver, AActor* target = nullptr, uint32 range = 0);

	AActor* GetTargetToAutoAttack() { return TargetToAutoAttack; }

	//Allow auto attack system to desengage already dead target
	void TargetHasDied(AActor* actor);

	void ForceAutoAttack(AActor* forcedTarget, uint32 forcedRange);

	void ProceedEventDeath();

	enum PlayerState
	{
		standard,
		noMovement,
		dead,
		stateCount
	};

	PlayerState state;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	//Count Mouse over event to be make sure when there is multiple mouse over events
	uint32 countMouseOverEvent;

	//True when click and player is moving near selected enemy to autoAtack
	bool bIsTargetingToAutoAttack;

	//Current target the auto Attack system is "aiming at"
	AActor* TargetToAutoAttack;

	//Save last target mouse has been over 
	AActor* LastTargetMouseOver;

	virtual void BeginPlay() override;

	void AutoAttackHandle();

	//Since each enemy has a different size
	//We need to store the range that is requiered
	//To reach the ennemy and do damage to it
	uint32 rangeToAttackToTarget;
	uint32 rangeToAttackToTargetLastMouseOver;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void HandleSkillInput(FKey keyPressed);

};


