// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwitchBlueprintToCPPGameMode.h"
#include "SwitchBlueprintToCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASwitchBlueprintToCPPGameMode::ASwitchBlueprintToCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/PFSPawn/BP_FPSCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
