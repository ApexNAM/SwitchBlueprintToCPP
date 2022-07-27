// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "AK_Weapon.h"
#include "AmmoHealthWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 1인칭 카메라 컴포넌트를 생성하고 생성한 컴포넌트 (카메라)를 캡슐 컴포넌트에 붙인다.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());

	// 카메라 위치를 변경 -> 폰 카메라 회전 제어 허용.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f,0.0f,50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(GetCapsuleComponent());

	BloodFTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	AimTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTimelineComponent"));
	
	// 1인칭 메시 컴포넌트 생성
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));

	// 소유 플레이어만 해당 메시를 볼 수 있음.
	FPSMesh->SetOnlyOwnerSee(true);

	// FPS 메시를 카메라에 붙인다.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// 일부 환경 그림자를 끄고 메시가 하나로 보이는 느낌을 유지.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);
	
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> FFPS_MPC(TEXT("MaterialParameterCollection'/Game/PFSPawn/FPS_MPC'"));

	if (FFPS_MPC.Succeeded())
	{
		FPS_MPC = FFPS_MPC.Object;
	}
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("We are FPSCharacter."));
	
	FActorSpawnParameters spawnWeaponParams;
	spawnWeaponParams.Owner = this;

	const FVector WeaponLocation = FPSMesh->GetComponentLocation();
	const FRotator WeaponRotation = FPSMesh->GetComponentRotation();

	FName PathP = TEXT("Blueprint'/Game/PFSPawn/BP_AK_Weapon.BP_AK_Weapon_C'");
	UClass* GeneratedPrimaryWeapon = Cast<UClass>(StaticLoadObject(UClass::StaticClass(),NULL,*PathP.ToString()));
	PrimaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(GeneratedPrimaryWeapon, WeaponLocation, WeaponRotation, spawnWeaponParams);

	if (PrimaryWeapon != nullptr)
	{
		PrimaryWeapon->AttachToComponent(FPSMesh,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true),
			TEXT("Palm_R_Socket"));
	}

	FName PathS = TEXT("Blueprint'/Game/PFSPawn/Glock_Weapon_BP.Glock_Weapon_BP_C'");
	UClass* GeneratedSeconderyWeapon = Cast<UClass>(StaticLoadObject(UClass::StaticClass(),NULL,*PathS.ToString()));
	SeconderyWeapon = GetWorld()->SpawnActor<AWeaponBase>(GeneratedSeconderyWeapon, WeaponLocation, WeaponRotation, spawnWeaponParams);

	if (SeconderyWeapon != nullptr)
	{
		SeconderyWeapon->AttachToComponent(FPSMesh,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true),
			TEXT("Palm_R_Socket"));

		SeconderyWeapon->GetRootComponent()->SetVisibility(false, false);
	}
	
	Health = 100;

	if (CurveFloatStart)
	{
		FOnTimelineFloat TimelineProgress;
		FOnTimelineEvent FinishTimeline;

		TimelineProgress.BindUFunction(this,FName("DamageTimeline"));
		
		BloodFTimeLine->AddInterpFloat(CurveFloatStart, TimelineProgress);
		BloodFTimeLine->SetTimelineFinishedFunc(FinishTimeline);
		
		BloodFTimeLine->SetTimelineLength(1.0f);
		BloodFTimeLine->SetLooping(false);
	}

	if (CurveFloatStart)
	{
		FOnTimelineFloat TimelineProgress;
		FOnTimelineEvent FinishTimeline;

		TimelineProgress.BindUFunction(this,FName("AimFieldMode"));
		AimTimeLine->AddInterpFloat(AimCurveFloat, TimelineProgress);
		AimTimeLine->SetTimelineFinishedFunc(FinishTimeline);

		BloodFTimeLine->SetTimelineLength(0.15f);
		BloodFTimeLine->SetLooping(false);
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward",this,&AFPSCharacter::PlayerMovementForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSCharacter::PlayerMovementRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse",this,&AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse",this,&AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,this,&AFPSCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released,this,&AFPSCharacter::OnStopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::Reload);

	PlayerInputComponent->BindAction("SelfDamage", IE_Pressed, this, &AFPSCharacter::Damage);

	PlayerInputComponent->BindAction("Switch_1", IE_Pressed, this, &AFPSCharacter::SwitchPrimaryWeapon);
	PlayerInputComponent->BindAction("Switch_2", IE_Pressed, this, &AFPSCharacter::SwitchSeconderyWeapon);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AFPSCharacter::OnAimDownSight);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AFPSCharacter::OffAimDownSight);
}

// 앞뒤로 움직이는 함수
void AFPSCharacter::PlayerMovementForward(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction,value);
}

// 좌우로 움직이는 함수
void AFPSCharacter::PlayerMovementRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction,value);
}

// 점프 시작
void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}

// 점프 종료
void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	if (!IsReloading)
	{
		UAnimInstance* AnimInstance = FPSMesh->GetAnimInstance();
		
		if (SwitchWeapon)
		{
			PrimaryWeapon->Fire();
		
			if (PrimaryWeapon->Ammoacount > 0)
			{
				if (!AimDownSight)
				{
					AnimInstance->Montage_Play(PrimaryWeaponAnim_Fire, 1.0f);
				}
				else
				{
					AnimInstance->Montage_Play(PrimaryWeaponAnim_Fire_ADS, 1.0f);
				}
			}
		}
		else
		{
			SeconderyWeapon->Fire();
		
			if (SeconderyWeapon->Ammoacount > 0)
			{
				if (!AimDownSight)
				{
					AnimInstance->Montage_Play(SeconderyWeaponAnim_Fire, 1.0f);
				}
				else
				{
					AnimInstance->Montage_Play(SeconderyWeaponAnim_Fire_ADS, 1.0f);
				}
			}
		}
	}
}

void AFPSCharacter::Reload()
{
	UAnimInstance* AnimInstance = FPSMesh->GetAnimInstance();

	if (SwitchWeapon)
	{
		if (PrimaryWeapon->Ammoacount < PrimaryWeapon->MaxAmmo)
		{
			if (!IsReloading)
			{
				IsReloading = true;
			
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(PrimaryWeaponAnim_Reload,1.0f);
				}

				PrimaryWeapon->Reload();

				FTimerHandle ReloadTimerHandle;
				GetWorldTimerManager().SetTimer(ReloadTimerHandle, this,
					&AFPSCharacter::ReloadFalse, PrimaryWeapon->ReloadTime, false);
			}
		}
	}
	else
	{
		if (SeconderyWeapon->Ammoacount < SeconderyWeapon->MaxAmmo)
		{
			if (!IsReloading)
			{
				IsReloading = true;
			
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(SeconderyWeaponAnim_Reload,1.0f);
				}

				SeconderyWeapon->Reload();

				FTimerHandle ReloadTimerHandle;
				GetWorldTimerManager().SetTimer(ReloadTimerHandle, this,
					&AFPSCharacter::ReloadFalse, SeconderyWeapon->ReloadTime, false);
			}
		}
	}
}

void AFPSCharacter::ReloadFalse()
{
	IsReloading = false;
}

void AFPSCharacter::Damage()
{
	if (Health > 0)
	{
		Health -= 10;
		BloodFTimeLine->PlayFromStart();
	}
}

void AFPSCharacter::DamageTimeline(float deltaTime)
{
	UKismetMaterialLibrary::SetScalarParameterValue(this, FPS_MPC, "VignetteAmount", deltaTime);
}

void AFPSCharacter::SwitchPrimaryWeapon()
{
	if (!AimDownSight)
	{
		if (!IsReloading)
		{
			SwitchWeapon = true;
		
			PrimaryWeapon->GetRootComponent()->SetVisibility(true, false);
			SeconderyWeapon->GetRootComponent()->SetVisibility(false, false);
		}
	}
}

void AFPSCharacter::SwitchSeconderyWeapon()
{
	if (!AimDownSight)
	{
		if (!IsReloading)
		{
			SwitchWeapon = false;
		
			PrimaryWeapon->GetRootComponent()->SetVisibility(false, false);
			SeconderyWeapon->GetRootComponent()->SetVisibility(true, false);
		}
	}
}

void AFPSCharacter::OnAimDownSight()
{
	AimDownSight = true;
	AimTimeLine->PlayFromStart();
}

void AFPSCharacter::OffAimDownSight()
{
	AimDownSight = false;
	AimTimeLine->PlayFromStart();
}

void AFPSCharacter::AimFieldMode(float value)
{
	if (AimDownSight)
	{
		FPSCameraComponent->SetFieldOfView(FMath::Lerp(90.0f,45.0f,value));

		const FVector StartLerp = FVector(1.6f,7.8f,-23.0f);
		const FVector EndLerp = FVector(1.6f,0.0f,-16.0f);
	
		const FVector AimMode = FMath::Lerp(StartLerp,EndLerp,value);
		const FRotator AimRot = FRotator(0.0f,-1.3f,0.0f);
		FPSMesh->SetRelativeTransform(FTransform(AimRot, AimMode));
	}
	else
	{
		FPSCameraComponent->SetFieldOfView(FMath::Lerp(45.0f,90.0f,value));

		const FVector StartLerp = FVector(1.6f,0.0f,-16.0f);
		const FVector EndLerp = FVector(1.6f,7.8f,-23.0f);
	
		const FVector AimMode = FMath::Lerp(StartLerp,EndLerp,value);
		const FRotator AimRot = FRotator(0.0f,6.4f,0.0f);
		FPSMesh->SetRelativeTransform(FTransform(AimRot, AimMode));
	}
}















