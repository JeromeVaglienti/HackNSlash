// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HacknSlashCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Skill/SkillManagerComponent.h"
#include "Engine/World.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "HacknSlashPlayerController.h"

AHacknSlashCharacter::AHacknSlashCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SkillManager = CreateDefaultSubobject<USkillManagerComponent>(TEXT("SkillManager"));

	stimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	TeamId = FGenericTeamId(10);
}

void AHacknSlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	HP = Max_HP = DefineHP;

	Range = 120.0f;

	if (widgetInfoDisplay != nullptr)
	{
		widgetInfoDisplay->UpdateLifeBar(GetHPPercentage(), HP, Max_HP);
	}

	playerControllerHnS = Cast<AHacknSlashPlayerController>(GetController());

	isAlreadyDead = false;
}

void AHacknSlashCharacter::TakeDamage(uint32 _damageValue, AActor* _damageMaker)
{
	IDamagable::TakeDamage(_damageValue, _damageMaker);

	widgetInfoDisplay->UpdateLifeBar(GetHPPercentage(), HP, Max_HP);

	//Death Handle
	if (HP == 0 && !isAlreadyDead)
	{
		Execute_HandleDeathOrDestruction(this);
		Cast<AHacknSlashPlayerController>(GetController())->ProceedEventDeath();
		isAlreadyDead = true;
	}
	else if (playerControllerHnS->GetTargetToAutoAttack() == nullptr)
	{
		//Force Auto Attack for the player
		playerControllerHnS->ForceAutoAttack(_damageMaker, 175);
	}
}

void AHacknSlashCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

FGenericTeamId AHacknSlashCharacter::GetGenericTeamId() const
{
	return TeamId;
}