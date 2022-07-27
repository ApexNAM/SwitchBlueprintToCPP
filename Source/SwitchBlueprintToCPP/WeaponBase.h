// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class SWITCHBLUEPRINTTOCPP_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* FirePoint;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	float ReloadTime; // 재장전 시간
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	float BulletSpread;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	int Ammoacount;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	int MaxAmmo;
	
	virtual void Fire();
	virtual void Reload();

	void SetAmmoMax();
};
