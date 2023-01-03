// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlayerGameMode.h"
#include "SlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASlayerGameMode::ASlayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_CharacterBase"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
