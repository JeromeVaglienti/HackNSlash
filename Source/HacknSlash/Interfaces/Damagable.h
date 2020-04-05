// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 Interface to allow to store life data and make damage easily on any actor you want
 */
class HACKNSLASH_API IDamagable
{
	GENERATED_BODY()
protected:

	uint32 HP;

	uint32 Max_HP;

	//Range needed to be damaged
	uint32 Range;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	FORCEINLINE uint32 GetHP() { return HP; }
	FORCEINLINE uint32 GetMaxHP() { return Max_HP; }
	FORCEINLINE bool isDead() { return HP == 0; }
	FORCEINLINE uint32 GetRange() { return Range; }

	float GetHPPercentage();

	virtual void TakeDamage(uint32 _damageValue, AActor* _damageMaker);

	//Implement in BP What To Do like death anim
	UFUNCTION(BlueprintImplementableEvent)
	void HandleDeathOrDestruction();
};
