// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent_Base.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Skill/SkillManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/Base_AI_Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "HacknSlash.h"

// Sets default values
AAgent_Base::AAgent_Base()
{
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 96.0f);
	
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	widgetComponentContainer = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	UAISenseConfig_Sight* AIsightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	if (widgetComponentContainer != nullptr)
	{
		widgetComponentContainer->SetupAttachment(RootComponent);
	}

	AIsightConfig->DetectionByAffiliation.bDetectEnemies = AIsightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIsightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->ConfigureSense(*AIsightConfig);
	AIPerceptionComponent->SetDominantSense(*AIsightConfig->GetSenseImplementation());

	SkillManagerAI = CreateDefaultSubobject<USkillManagerComponent>(TEXT("SkillManagerAI"));

	Range = 175.0f;
}

// Called when the game starts or when spawned
void AAgent_Base::BeginPlay()
{
	Super::BeginPlay();

	if (widgetComponentContainer != nullptr)
	{
		widgetComponentContainer->InitWidget();
	}

	HP = Max_HP = DefineHP;

	if (widgetComponentContainer != nullptr && widgetComponentContainer->GetUserWidgetObject() != nullptr)
	{
		widgetInfoDisplay = Cast<UAI_Widget>(widgetComponentContainer->GetUserWidgetObject());
		widgetInfoDisplay->UpdateLifeBar(GetHPPercentage());
	}
	else
	{
		widgetInfoDisplay = nullptr;
		UE_LOG(LogHacknSlashError, Error, TEXT("Widget class not set in widget component %s"), *GetNameSafe(this));
	}

	if (AIPerceptionComponent != nullptr)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAgent_Base::HandleAIPerceptionUpdate);
	}

	m_BC_Attached->SetValueAsVector(FName("HomeLocation"), GetActorLocation());
	m_BC_Attached->SetValueAsBool(FName("IsAtHome"), true);


	hasTarget = false;
	isAlreadyDead = false;
}

void AAgent_Base::HandleAIPerceptionUpdate(AActor* Actor, FAIStimulus Stimuluss)
{
	if (Stimuluss.WasSuccessfullySensed())
	{
		m_BC_Attached->SetValueAsObject(FName("Player"), Actor);
		m_BC_Attached->SetValueAsBool(FName("IsAtHome"), false);
		hasTarget = true;
	}
	else
	{
		m_BC_Attached->SetValueAsObject(FName("Player"), NULL);
		hasTarget = false;
	}
}

void AAgent_Base::TakeDamage(uint32 _damageValue, AActor* _damageMaker)
{
	IDamagable::TakeDamage(_damageValue, _damageMaker);

	if(widgetInfoDisplay != nullptr)
	{
		widgetInfoDisplay->UpdateLifeBar(GetHPPercentage());
	}
	
	//Death handle
	if (HP == 0 && !isAlreadyDead)
	{
		//Visual called handle in the blueprint
		Execute_HandleDeathOrDestruction(this);

		//Set up controller for the dead state of the character
		Cast<ABase_AI_Controller>(GetController())->ProceedEventDeath();
		widgetInfoDisplay->SetVisibility(ESlateVisibility::Hidden);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetGenerateOverlapEvents(false);
		GetCapsuleComponent()->Deactivate();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetGenerateOverlapEvents(false);
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AAgent_Base::HandleAIPerceptionUpdate);
		isAlreadyDead = true;
	}
	else if (!hasTarget)
	{
		//Auto attack when is attacked
		//Add a more fluid gameplay by doing so and also weird behavior
		//Like a AI not responding when it is being attack from behind
		GetController()->StopMovement();
		m_BC_Attached->SetValueAsObject(FName("Player"), _damageMaker);
	}
}

void AAgent_Base::GetDataToBlackBoard(UBlackboardComponent * _pbbData)
{
	m_BC_Attached = _pbbData;
}

// Called every frame
void AAgent_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAgent_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}