// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SkillsCollisionDetectionGeneric.h"
#include "SkillLinearProjectile.generated.h"


UCLASS()
class HACKNSLASH_API ASkillLinearProjectile : public AActor, public ISkillsCollisionDetectionGeneric
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties	
	ASkillLinearProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* projectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkillBase* skillOwner;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* projectileMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float distanceTravelled = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRange = 0.0f;

	FVector lastFramePosition;

	virtual void Init(USkillBase* _skillOwner, FVector _direction, float _speed, float _maxRange);

	UFUNCTION()
	void ProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void ProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
