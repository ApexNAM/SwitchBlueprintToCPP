// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class SWITCHBLUEPRINTTOCPP_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
	UPROPERTY()
	UUserWidget* CurrentCrossHairWidget;

	UPROPERTY()
	UUserWidget* CurrentUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget")
	TSubclassOf<UUserWidget> CrossHairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget")
	TSubclassOf<UUserWidget> UIWidget;

public:
	void SetCrossHairWidget();
	void SetUIWidget();
};
