// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "AI/Agent_Base.h"
#include "AI_Spawner.generated.h"

UCLASS()


class HACKNSLASH_API AAI_Spawner : public AActor
{
	GENERATED_BODY()

	UNavigationSystemV1* m_pCurrentNavigationSystem;

	float m_ftimerCount;
	void Spawn();

	// Sets default values for this actor's properties
	AAI_Spawner();

	uint32 AISpawned;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "AI Spawned"))
	TSubclassOf<AAgent_Base> AgentTypeToSpawn;

	UPROPERTY(EditAnywhere, meta = (Category = "Spawn", DisplayName = "Radius", ClampMin = "5"))
	float m_fSpawnRadius = 100.0f;

	UPROPERTY(EditAnywhere, meta = (Category = "Spawn", DisplayName = "Rate", ClampMin = "0.01"))
	float m_fSpawnRate = 3.0f;

	UPROPERTY(EditAnywhere, meta = (Category = "Spawn", DisplayName = "Count", ClampMax = "100"))
	uint32 count;

};
