// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobGrapplingHookGameMode.h"
#include "MobGrapplingHookCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMobGrapplingHookGameMode::AMobGrapplingHookGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
