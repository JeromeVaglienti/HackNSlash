// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AI_Widget.generated.h"

/**
 * 
 */
UCLASS()
class HACKNSLASH_API UAI_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateLifeBar(float percentage);

};
