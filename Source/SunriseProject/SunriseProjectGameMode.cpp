// Copyright Epic Games, Inc. All Rights Reserved.

#include "SunriseProjectGameMode.h"
#include "SunriseProjectPawn.h"

ASunriseProjectGameMode::ASunriseProjectGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASunriseProjectPawn::StaticClass();
}

