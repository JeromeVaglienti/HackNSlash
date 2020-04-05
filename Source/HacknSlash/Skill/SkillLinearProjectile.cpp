// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillLinearProjectile.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Skill/SkillBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/Damagable.h"
#include "Skill/SkillManagerComponent.h"
#include "Interfaces/SkillsCollisionDetectionGeneric.h"

// Sets default values
ASkillLinearProjectile::ASkillLinearProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("DetectionMesh");

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	if (projectileMesh != nullptr && RootComponent != nullptr && projectileMovementComponent != nullptr)
	{
		projectileMesh->SetupAttachment(RootComponent);
		projectileMovementComponent->SetUpdatedComponent(RootComponent);
		projectileMovementComponent->bInitialVelocityInLocalSpace = false;
		projectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void ASkillLinearProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (projectileMesh != nullptr)
	{
		projectileMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASkillLinearProjectile::ProjectileBeginOverlap);
		projectileMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &ASkillLinearProjectile::ProjectileEndOverlap);
	}

}

// Called every frame
void ASkillLinearProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	distanceTravelled += FVector::Distance(lastFramePosition, GetActorLocation());
	lastFramePosition = GetActorLocation();

	if (distanceTravelled >= maxRange)
	{
		skillOwner->OnDetectionNegative();
		Destroy();
	}
}

void ASkillLinearProjectile::Init(USkillBase * _skillOwner, FVector _direction, float _speed, float _maxRange)
{
	skillOwner = _skillOwner;
	distanceTravelled = 0.0f;
	maxRange = _maxRange;
	lastFramePosition = GetActorLocation();

	if (skillOwner == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ASkillLinearProjectile::Init :: NO SKILL OWNER")));

	if (projectileMovementComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ASkillLinearProjectile::Init :: NO PROJECTILE MOVE COMP")));
		return;
	}

	if (!_direction.IsNearlyZero())
	{
		_direction.Normalize();
		projectileMovementComponent->Velocity = _direction * _speed;
	}
}

void ASkillLinearProjectile::ProjectileBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//Avoid hitting ourself
	if (OtherActor == skillOwner->SkillMgrOwner->GetOwner())
	{
		return;
	}
	
	if (skillOwner != nullptr && Cast<ISkillsCollisionDetectionGeneric>(OtherActor) == NULL)
	{
		skillOwner->OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult, this);
		Destroy();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ASkillLinearProjectile:: BeginOverlap CPP")));
}

void ASkillLinearProjectile::ProjectileEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (skillOwner != nullptr)
		skillOwner->OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ASkillLinearProjectile:: EndOverlap CPP")));
}

