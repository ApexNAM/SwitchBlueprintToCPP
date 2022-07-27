// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"

#include "FPSCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();
	SetCrossHairWidget();
}

void AFPSHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetUIWidget();
}



void AFPSHUD::SetCrossHairWidget()
{
	if (CurrentCrossHairWidget != nullptr)
	{
		CurrentCrossHairWidget->RemoveFromViewport();
		CurrentCrossHairWidget = nullptr;
	}
	
	if (CrossHairWidget != nullptr)
	{
		CurrentCrossHairWidget = CreateWidget(GetWorld(),CrossHairWidget);

		if (CurrentCrossHairWidget != nullptr)
		{
			CurrentCrossHairWidget->AddToViewport();			
		}
	}
}

void AFPSHUD::SetUIWidget()
{
	if (CurrentUIWidget != nullptr)
	{
		CurrentUIWidget->RemoveFromViewport();
		CurrentUIWidget = nullptr;
	}
	
	if (UIWidget != nullptr)
	{
		CurrentUIWidget = CreateWidget(GetWorld(),UIWidget);

		if (CurrentUIWidget != nullptr)
		{
			CurrentUIWidget->AddToViewport();			
		}
	}
}


