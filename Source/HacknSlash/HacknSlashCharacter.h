// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damagable.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerWidget.h"
#include "Components/WidgetComponent.h"
#include "HacknSlashCharacter.generated.h"

class USkillManagerComponent;
class UAIPerceptionStimuliSourceComponent;
class AHacknSlashPlayerController;

UCLASS(Blueprintable)
class AHacknSlashCharacter : public ACharacter, public IDamagable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AHacknSlashCharacter();

	enum IDSKILLS
	{
		autoAttack,
		fireSkill,
		ultimateSkill,
		countSkill
	};
private:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/

	virtual void TakeDamage(uint32 _damageValue, AActor* _damageMaker) override;

	AHacknSlashPlayerController* playerControllerHnS;
public:

	UPROPERTY(EditAnywhere)
	UAIPerceptionStimuliSourceComponent* stimuliSource;

	UPROPERTY(EditAnywhere)
	class USkillManagerComponent* SkillManager;

	FGenericTeamId GetGenericTeamId() const;

	UPROPERTY(BlueprintReadWrite)
	UPlayerWidget* widgetInfoDisplay;

	UPROPERTY(EditAnywhere)
	uint32 DefineHP;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FGenericTeamId TeamId;

	virtual void BeginPlay() override;

	//Avoid firing death sequence multiple times
	bool isAlreadyDead;
};

