// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT2096Assignment3GameMode.h"
#include "FIT2096Assignment3Character.h"
#include "UObject/ConstructorHelpers.h"

AFIT2096Assignment3GameMode::AFIT2096Assignment3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
