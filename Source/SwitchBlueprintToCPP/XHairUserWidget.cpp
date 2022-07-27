// Fill out your copyright notice in the Description page of Project Settings.


#include "XHairUserWidget.h"

#include "FPSCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

UXHairUserWidget::UXHairUserWidget(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	InterpSpeed = 15.0f;
	UpperBound = -80.0f;
	LowerBound = -25.0f;
}

void UXHairUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FTimerHandle CrossHairTimeHandle;
	
	GetWorld()->GetTimerManager().SetTimer(
	CrossHairTimeHandle,[&]()
	{
		SetCrossHair();
	},
	0.01f,true
	);

	GetWorld()->GetTimerManager().SetTimer(CrossHairTimeHandle, [&]()
	{
		HideDuringADS();
	},
	0.1f,true
	);
}

void UXHairUserWidget::SetCrossHair()
{
	UCanvasPanelSlot* TopCanvas = Cast<UCanvasPanelSlot>(Top->Slot);
	UCanvasPanelSlot* BottonCanvas = Cast<UCanvasPanelSlot>(Botton->Slot);
	UCanvasPanelSlot* LeftCanvas = Cast<UCanvasPanelSlot>(Left->Slot);
	UCanvasPanelSlot* RightCanvas = Cast<UCanvasPanelSlot>(Right->Slot);
	
	AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	float VelocitySize = FPSCharacter->GetVelocity().Size() * -1.0f;

	float TopBottonX = FMath::FInterpTo(TopCanvas->GetPosition().Y,VelocitySize,GetWorld()->GetDeltaSeconds(),InterpSpeed);
	float LeftRightX =  FMath::FInterpTo(LeftCanvas->GetPosition().X,VelocitySize,GetWorld()->GetDeltaSeconds(),InterpSpeed);
	
	float TopClampY = FMath::Clamp(TopBottonX,UpperBound, LowerBound);
	float BottonClampY = TopClampY * -1.0f;
	
	float LeftClampX = FMath::Clamp(LeftRightX, UpperBound, LowerBound);
	float RightClampX = LeftClampX * -1.0f;
	
	TopCanvas->SetPosition(FVector2d(0.0f,TopClampY));
	BottonCanvas->SetPosition(FVector2d(0.0f ,BottonClampY));

	LeftCanvas->SetPosition(FVector2d(LeftClampX, 0.0f));
	RightCanvas->SetPosition(FVector2d(RightClampX, 0.0f));
}

void UXHairUserWidget::HideDuringADS()
{
	AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	if (FPSCharacter->AimDownSight)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
}

