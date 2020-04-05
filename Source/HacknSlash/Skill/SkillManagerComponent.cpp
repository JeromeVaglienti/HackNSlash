// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManagerComponent.h"
#include "HacknSlashCharacter.h"

// Sets default values for this component's properties
USkillManagerComponent::USkillManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < Skills.Num(); i++)
	{
		Skills[i]->SkillMgrOwner = this;
		Skills[i]->InitSkill();
	}
}


// Called every frame
void USkillManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int i = 0; i < Skills.Num(); i++)
	{
		if (Skills[i] != nullptr && Skills[i]->IsSkillExecuting())
		{
			Skills[i]->TickSkill(DeltaTime);
		}
	}
}

bool USkillManagerComponent::IsASkillExecuting(const int32 _skillIndex)
{
	for (int i = 0; i < Skills.Num(); i++)
	{
		if (Skills[i]->IsSkillExecuting() && (!Skills[i]->CanBeOverrideByOtherSkill || i == _skillIndex))
		{
			return true;
		}
	}
	return false;
}

void USkillManagerComponent::OnSkillRequested(const int32 _skillIndex)
{
	if (_skillIndex < Skills.Num() && !IsASkillExecuting(_skillIndex))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(" SKILL " + FString::FromInt(_skillIndex)) + "  " + GetOwner()->GetFName().ToString());
		Skills[_skillIndex]->RequestExecution();
	}
}
