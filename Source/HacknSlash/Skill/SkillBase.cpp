// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "Skill/SkillManagerComponent.h"

void USkillBase::Execute()
{
	isExecuting = true;

	Event_ExecuteBP();

	if (bSkillIsOneTickOnly)
	{
		EndExecution();
	}
}

void USkillBase::EndExecution()
{
	isExecuting = false;

	Event_EndExecutionBP();

	cooldownTimer = cooldown;
}

bool USkillBase::RequestExecution()
{
	if (cooldownTimer == cooldown)
	{
		Execute();
		return true;
	}
	else
	{
		return false;
	}
}

void USkillBase::TickSkill(float _deltaTime)
{
	if (cooldownTimer > 0.0f)
	{
		Event_TickSkillBP(_deltaTime);

		cooldownTimer -= _deltaTime;
	}
}

void USkillBase::InitSkill()
{
	isExecuting = false;
}
