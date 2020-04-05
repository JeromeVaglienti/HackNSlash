// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SkillsCollisionDetectionGeneric.h"
#include "SkillCollisionDetection.generated.h"


UCLASS()
class HACKNSLASH_API ASkillCollisionDetection : public AActor, public ISkillsCollisionDetectionGeneric
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties
	ASkillCollisionDetection();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* detectionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkillBase* skillOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(USkillBase* _skillOwner);

	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
