// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "BreakableRock.generated.h"


class AHacknSlashPlayerController;

UCLASS()
class HACKNSLASH_API ABreakableRock : public AActor, public IDamagable
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ABreakableRock();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* rockMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AHacknSlashPlayerController* m_pPlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(uint32 _damageValue, AActor* _damageMaker) override;

	UPROPERTY(EditAnywhere)
	uint32 DefineHP;

private:

	UFUNCTION()
	void ClickedEventBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void ClickedEventEndMouseOver(UPrimitiveComponent* TouchedComponent);

	bool bIsMouseOverred;

};
