// Copyright Epic Games, Inc. All Rights Reserved.

#include "AudioProjectMasteredGameMode.h"
#include "AudioProjectMasteredCharacter.h"
#include "EndScreen.h"
#include "UObject/ConstructorHelpers.h"

AAudioProjectMasteredGameMode::AAudioProjectMasteredGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
