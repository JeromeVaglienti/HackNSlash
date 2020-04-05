// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Spawner.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AAI_Spawner::AAI_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAI_Spawner::BeginPlay()
{
	Super::BeginPlay();

	m_ftimerCount = 0.0f;

	m_pCurrentNavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	AISpawned = 0;
}

// Called every frame
void AAI_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_ftimerCount += DeltaTime;

	if (m_ftimerCount >= m_fSpawnRate && AISpawned < count)
	{
		Spawn();
		m_ftimerCount -= m_fSpawnRate;
		AISpawned++;
	}
}

void AAI_Spawner::Spawn()
{
	FNavLocation result;
	if (m_pCurrentNavigationSystem->GetRandomPointInNavigableRadius(GetActorLocation(), m_fSpawnRadius, result))
	{
		APawn* newAI = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), AgentTypeToSpawn.Get(), NULL, result.Location);
	}
}

