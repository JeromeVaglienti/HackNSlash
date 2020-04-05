// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HacknSlashGameMode.h"
#include "HacknSlashPlayerController.h"
#include "HacknSlashCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHacknSlashGameMode::AHacknSlashGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AHacknSlashPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/HacknSlash/MainCharacter/HacknSlashCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}