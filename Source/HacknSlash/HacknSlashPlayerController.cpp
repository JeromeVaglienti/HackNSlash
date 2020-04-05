// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HacknSlashPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Skill/SkillManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "HacknSlashCharacter.h"
#include "Engine/World.h"

AHacknSlashPlayerController::AHacknSlashPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableMouseOverEvents = true;
}

void AHacknSlashPlayerController::EnnemyMouseOverHandle(bool isOver, AActor* target, uint32 range)
{
	if (isOver)
	{
		countMouseOverEvent++;
		LastTargetMouseOver = target;
		rangeToAttackToTargetLastMouseOver = range;
	}
	else if(countMouseOverEvent > 1)
	{
		countMouseOverEvent--;
	}

	if (!isOver && LastTargetMouseOver == target)
	{
		LastTargetMouseOver = nullptr;
		rangeToAttackToTargetLastMouseOver = 0;
		CurrentMouseCursor = EMouseCursor::Crosshairs;
	}
	else
	{
		CurrentMouseCursor = EMouseCursor::CardinalCross;
	}
}

void AHacknSlashPlayerController::TargetHasDied(AActor * actor)
{
	TargetToAutoAttack = nullptr;
	rangeToAttackToTarget = 0;
}

void AHacknSlashPlayerController::ForceAutoAttack(AActor * forcedTarget, uint32 forcedRange)
{
	rangeToAttackToTargetLastMouseOver = forcedRange;
	LastTargetMouseOver = forcedTarget;
	AutoAttackHandle();
}

void AHacknSlashPlayerController::BeginPlay()
{
	Super::BeginPlay();
	countMouseOverEvent = 0;
	bIsTargetingToAutoAttack = false;
	LastTargetMouseOver = TargetToAutoAttack = nullptr;
	state = PlayerState::standard;
}

void AHacknSlashPlayerController::ProceedEventDeath()
{
	state = PlayerState::dead;
	StopMovement();
	bShowMouseCursor = false;
	InputComponent->ClearActionBindings();
}

void AHacknSlashPlayerController::AutoAttackHandle()
{
	if (LastTargetMouseOver != nullptr)
	{
		TargetToAutoAttack = LastTargetMouseOver;
		rangeToAttackToTarget = rangeToAttackToTargetLastMouseOver;
		bIsTargetingToAutoAttack = true;
		SetNewMoveDestination(TargetToAutoAttack->GetActorLocation());
	}
}

void AHacknSlashPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (state != PlayerState::standard)
	{
		return;
	}

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
	else if (bIsTargetingToAutoAttack && TargetToAutoAttack != nullptr)
	{
		SetNewMoveDestination(TargetToAutoAttack->GetActorLocation());
	}
}

void AHacknSlashPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AHacknSlashPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AHacknSlashPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("AutoAttack", IE_Pressed, this, &AHacknSlashPlayerController::AutoAttackHandle);

	InputComponent->BindAction("Skill", IE_Pressed, this, &AHacknSlashPlayerController::HandleSkillInput);
}

void AHacknSlashPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void AHacknSlashPlayerController::SetNewMoveDestination(const FVector DestLocation)
{	
	APawn* const MyPawn = GetPawn();
	
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		if (bIsTargetingToAutoAttack)
		{
			if ((Distance < rangeToAttackToTarget))
			{
				//Make sure the character is looking at the target directly to make the attack realistic
				MyPawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(), DestLocation));
				StopMovement();
				(Cast<AHacknSlashCharacter>(GetCharacter()))->SkillManager->OnSkillRequested(AHacknSlashCharacter::IDSKILLS::autoAttack);
			}
			else
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
			}
		}
		else if ((Distance > 120.0f)) // We need to issue move command only if far enough in order for walk animation to play correctly
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AHacknSlashPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
	bIsTargetingToAutoAttack = false;
	TargetToAutoAttack = nullptr;
}

void AHacknSlashPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AHacknSlashPlayerController::HandleSkillInput(FKey keyPressed)
{	
	if (keyPressed == EKeys::A)
	{
		(Cast<AHacknSlashCharacter>(GetCharacter()))->SkillManager->OnSkillRequested(AHacknSlashCharacter::IDSKILLS::fireSkill);
	}
	else if (keyPressed == EKeys::Z)
	{
		(Cast<AHacknSlashCharacter>(GetCharacter()))->SkillManager->OnSkillRequested(AHacknSlashCharacter::IDSKILLS::ultimateSkill);
	}
}
