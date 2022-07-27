// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHealthWidget.h"

#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAmmoHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetHealth();
	GetAmmocount();
	GetMaxAmmo();
}

void UAmmoHealthWidget::GetAmmocount()
{
	AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	if (PlayerController)
	{
		if (CurrentAmmo_Text)
		{
			if (PlayerController->SwitchWeapon)
			{
				FString AmmoCount1 = FString::FromInt(PlayerController->PrimaryWeapon->Ammoacount);
				CurrentAmmo_Text->SetText(FText::FromString(AmmoCount1));
			}
			else
			{
				FString AmmoCount2 = FString::FromInt(PlayerController->SeconderyWeapon->Ammoacount);
				CurrentAmmo_Text->SetText(FText::FromString(AmmoCount2));
			}
		}
	}
}

void UAmmoHealthWidget::GetMaxAmmo()
{
	AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	if (PlayerController)
	{
		if (MaxAmmo_Text)
		{
			if (PlayerController->SwitchWeapon)
			{
				FString MaxAmmo1 = FString::FromInt(PlayerController->PrimaryWeapon->MaxAmmo);
				MaxAmmo_Text->SetText(FText::FromString(MaxAmmo1));
			}
			else
			{
				FString MaxAmmo2 = FString::FromInt(PlayerController->SeconderyWeapon->MaxAmmo);
				MaxAmmo_Text->SetText(FText::FromString(MaxAmmo2));
			}
		}
	}
}

void UAmmoHealthWidget::GetHealth()
{
	AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	if (PlayerController)
	{
		if (Health_Text)
		{
			FString HealthString = FString::FromInt(PlayerController->Health);
			Health_Text->SetText(FText::FromString(HealthString));
		}
	}
}


