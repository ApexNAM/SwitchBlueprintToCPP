// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Components/PointLightComponent.h"
#include "AK_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SWITCHBLUEPRINTTOCPP_API AAK_Weapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	AAK_Weapon();

	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, Category="Anim")
	UAnimationAsset* FireAnimAsset;
	
	UPROPERTY(VisibleAnywhere, Category="Anim")
	UAnimationAsset* ReloadAnimAsset;
	
public:
	UPROPERTY(VisibleAnywhere, Category="Sound")
	USoundBase* FireSound;

	UPROPERTY(VisibleAnywhere, Category="Sound")
	USoundBase* DrySound;

public:
	UPROPERTY(VisibleAnywhere, Category="Sound")
	USoundAttenuation* AK_Attenuation;
	
	UPROPERTY(VisibleAnywhere, Category="Sound")
	USoundConcurrency* AK_Concurrency;

public:
	UPROPERTY(VisibleAnywhere, Category="Flash")
	UPointLightComponent* PointLight;
	
	UPROPERTY(VisibleAnywhere, Category="Flash")
	UStaticMeshComponent* MuzzleFlash;

public:
	virtual void Fire() override;
	virtual void Reload() override;
	
public:
	void AmmoCheck();
	void LineTrace();
	void SpawnDecalTracer(const FVector Location, const FVector SpawnTransformLocation, const FVector ImpactPoint);
	void MuzzleFlashFunction();
	void MuzzleFlashEnd() const;
};
