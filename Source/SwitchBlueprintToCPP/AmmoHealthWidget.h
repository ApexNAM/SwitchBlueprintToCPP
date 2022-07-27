// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AmmoHealthWidget.generated.h"

/**
 * 
 */

UCLASS()
class SWITCHBLUEPRINTTOCPP_API UAmmoHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmo_Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxAmmo_Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Health_Text;
public:
	void GetAmmocount();
	void GetMaxAmmo();
	void GetHealth();
};
