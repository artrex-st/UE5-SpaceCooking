// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceCookingGameMode.h"
#include "SpaceCookingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpaceCookingGameMode::ASpaceCookingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
