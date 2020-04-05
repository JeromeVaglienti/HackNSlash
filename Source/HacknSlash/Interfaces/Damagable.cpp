// Fill out your copyright notice in the Description page of Project Settings.


#include "Damagable.h"

// Add default functionality here for any IDamagable functions that are not pure virtual.

float IDamagable::GetHPPercentage()
{
	return (HP / (float)Max_HP);
}

void IDamagable::TakeDamage(uint32 _damageValue, AActor* _damageMaker)
{
	if (HP <= _damageValue)
	{
		HP = 0;
	}
	else
	{
		HP -= _damageValue;
	}
}
