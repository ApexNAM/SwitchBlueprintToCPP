// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AK_Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class SWITCHBLUEPRINTTOCPP_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 좌우앞뒤 움직임
	void PlayerMovementForward(float value);
	void PlayerMovementRight(float value);

	// 점프 시작 및 멈춤
	void OnStartJump();
	void OnStopJump();

	void Fire();
	
	void Reload();
	void ReloadFalse();
	void Damage();

	void SwitchPrimaryWeapon();
	void SwitchSeconderyWeapon();

	void OnAimDownSight();
	void OffAimDownSight();

	UFUNCTION()
	void DamageTimeline(float deltaTime);

	UFUNCTION()
	void AimFieldMode(float value);

	UPROPERTY(VisibleAnywhere, Category="Health")
	int Health;
	
	bool IsReloading;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Aim")
	bool AimDownSight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Health")
	bool SwitchWeapon = true;
public:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category= "FPSMesh")
	USkeletalMeshComponent* FPSMesh;

	int WeaponEquipped = 0;
public:
	UPROPERTY(VisibleAnywhere)
	AWeaponBase* PrimaryWeapon;

	UPROPERTY(VisibleAnywhere)
	AWeaponBase* SeconderyWeapon;

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* PrimaryWeaponAnim_Fire;

	UPROPERTY(EditAnywhere)
	UAnimMontage* PrimaryWeaponAnim_Fire_ADS;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* SeconderyWeaponAnim_Reload;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* SeconderyWeaponAnim_Fire;

	UPROPERTY(EditAnywhere)
	UAnimMontage* SeconderyWeaponAnim_Fire_ADS;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* PrimaryWeaponAnim_Reload;

public:
	UPROPERTY(VisibleAnywhere)
	UPostProcessComponent* PostProcess;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TimeLine", meta=(AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloatStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TimeLine", meta=(AllowPrivateAccess = "true"))
	UCurveFloat* AimCurveFloat;
	
	UPROPERTY(VisibleAnywhere)
	UMaterialParameterCollection* FPS_MPC;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* BloodFTimeLine;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* AimTimeLine;
};
