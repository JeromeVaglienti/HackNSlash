// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollisionDetection.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillManagerComponent.h"

// Sets default values
ASkillCollisionDetection::ASkillCollisionDetection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	detectionMesh = CreateDefaultSubobject<UStaticMeshComponent>("DetectionMesh");

	if (detectionMesh != nullptr && RootComponent != nullptr)
		detectionMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASkillCollisionDetection::BeginPlay()
{
	Super::BeginPlay();

	if (detectionMesh != nullptr)
	{
		detectionMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASkillCollisionDetection::TriggerBeginOverlap);
		detectionMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &ASkillCollisionDetection::TriggerEndOverlap);
	}
}

// Called every frame
void ASkillCollisionDetection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillCollisionDetection::Init(USkillBase * _skillOwner)
{
	skillOwner = _skillOwner;

	if (skillOwner == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SkillCollisionDetection::Init :: NO SKILL OWNER")));
}

void ASkillCollisionDetection::TriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == skillOwner->SkillMgrOwner->GetOwner())
	{
		return;
	}

	if (skillOwner != nullptr)
		skillOwner->OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult, this);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SkillCollisionDetection:: BeginOverlap CPP")));
}

void ASkillCollisionDetection::TriggerEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (skillOwner != nullptr)
		skillOwner->OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SkillCollisionDetection:: EndOverlap CPP")));
}

