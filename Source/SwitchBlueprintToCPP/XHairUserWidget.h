// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "XHairUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SWITCHBLUEPRINTTOCPP_API UXHairUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXHairUserWidget(const FObjectInitializer &ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Top;

	UPROPERTY(meta = (BindWidget))
	UImage* Botton;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Left;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Right;

public:
	float InterpSpeed;
	float UpperBound;
	float LowerBound;
public:
	UFUNCTION()
	void SetCrossHair();
	
	UFUNCTION()
	void HideDuringADS();
};
