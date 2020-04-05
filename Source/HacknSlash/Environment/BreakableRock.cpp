// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakableRock.h"
#include "Engine/World.h"
#include "HacknSlashPlayerController.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABreakableRock::ABreakableRock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rockMesh"));
}

// Called when the game starts or when spawned
void ABreakableRock::BeginPlay()
{
	Super::BeginPlay();
	
	m_pPlayerController = Cast<AHacknSlashPlayerController>(GetWorld()->GetFirstPlayerController());

	bIsMouseOverred = false;

	rockMesh->OnBeginCursorOver.AddDynamic(this, &ABreakableRock::ClickedEventBeginMouseOver);

	rockMesh->OnEndCursorOver.AddDynamic(this, &ABreakableRock::ClickedEventEndMouseOver);

	HP = Max_HP = DefineHP;
	Range = 450.0f;
}

// Called every frame
void ABreakableRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableRock::TakeDamage(uint32 _damageValue, AActor * _damageMaker)
{
	IDamagable::TakeDamage(_damageValue, _damageMaker);

	if (HP == 0)
	{
		rockMesh->SetCanEverAffectNavigation(false);

		if (bIsMouseOverred)
		{
			m_pPlayerController->EnnemyMouseOverHandle(false);
		}
		m_pPlayerController->TargetHasDied(this);

		Execute_HandleDeathOrDestruction(this);
	}
}

void ABreakableRock::ClickedEventBeginMouseOver(UPrimitiveComponent * TouchedComponent)
{
	m_pPlayerController->EnnemyMouseOverHandle(true, this, Range);
	bIsMouseOverred = true;
}

void ABreakableRock::ClickedEventEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	m_pPlayerController->EnnemyMouseOverHandle(false);
	bIsMouseOverred = false;
}
