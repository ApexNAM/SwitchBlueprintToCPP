// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameModeBase.h"

#include "FPSHUD.h"

AFPSGameModeBase::AFPSGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> FPSPawnBPClass(TEXT("/Game/PFSPawn/BP_FPSCharacter"));
	if (FPSPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = FPSPawnBPClass.Class;
	}

	static  ConstructorHelpers::FClassFinder<AHUD> FPSHUDBPClass(TEXT("Blueprint'/Game/PFSPawn/FPSHUD_BP'"));
	HUDClass = FPSHUDBPClass.Class;
}
