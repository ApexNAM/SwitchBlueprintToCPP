// Fill out your copyright notice in the Description page of Project Settings.


#include "AK_Weapon.h"

#include "BulletDecal.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAK_Weapon::AAK_Weapon()
{
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	FirePoint->SetupAttachment(Mesh);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(Mesh);
	PointLight->Intensity = 0.0f;
	
	MuzzleFlash = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleFlash"));
	MuzzleFlash->SetupAttachment(Mesh);

	static ConstructorHelpers::FObjectFinder<UMaterial>FMuzzleFlash(TEXT("Material'/Game/PFSPawn/MuzzleFlash_MAT'"));

	if (FMuzzleFlash.Succeeded())
	{
		MuzzleFlash->SetMaterial(0,FMuzzleFlash.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FMuzzleFlashMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane'"));

	if (FMuzzleFlashMesh.Succeeded())
	{
		MuzzleFlash->SetStaticMesh(FMuzzleFlashMesh.Object);
		MuzzleFlash->SetRelativeRotation(FRotator(-90.0f,180.0f,0.0f));
		MuzzleFlash->SetRelativeScale3D(FVector(0.5f,0.5f,0.5f));
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AkMesh(TEXT("SkeletalMesh'/Game/PFSPawn/FPSPawnFiles/Models/AK_rigged'"));

	if (AkMesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(AkMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> AkMaterial(TEXT("Material'/Game/PFSPawn/FPSPawnFiles/Textures/AK_D_Mat'"));

	if (AkMaterial.Succeeded())
	{
		Mesh->SetMaterial(0,AkMaterial.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AKFire(TEXT("AnimMontage'/Game/PFSPawn/FPSPawnFiles/Anims/AK_Fire_anim_Montage'"));

	if(AKFire.Succeeded())
	{
		FireAnimAsset = AKFire.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AKReload(TEXT("AnimMontage'/Game/PFSPawn/FPSPawnFiles/Anims/AK_Reload_anim_Montage'"));

	if (AKReload.Succeeded())
	{
		ReloadAnimAsset = AKReload.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FFireSound(TEXT("SoundWave'/Game/PFSPawn/FPSPawnFiles/Sounds/Gunshot'"));

	if (FFireSound.Succeeded())
	{
		FireSound = FFireSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FDrySound(TEXT("SoundWave'/Game/PFSPawn/FPSPawnFiles/Sounds/DryFire'"));

	if (FDrySound.Succeeded())
	{
		DrySound = FDrySound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> FFireAttenuation(TEXT("SoundAttenuation'/Game/PFSPawn/FPSPawnFiles/AK_Attenuation'"));

	if (FFireAttenuation.Succeeded())
	{
		AK_Attenuation = FFireAttenuation.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundConcurrency> FFireConcurrency(TEXT("SoundConcurrency'/Game/PFSPawn/FPSPawnFiles/AKSoundConcurrency'"));

	if (FFireConcurrency.Succeeded())
	{
		AK_Concurrency = FFireConcurrency.Object;
	}
}

void AAK_Weapon::BeginPlay()
{
	Super::BeginPlay();

	MaxAmmo = 30;
	Ammoacount = MaxAmmo;
	
	ReloadTime = 2.5f;
	BulletSpread = 500.0f;
}

void AAK_Weapon::Fire()
{
	if (Ammoacount > 0)
	{
		AmmoCheck();
		LineTrace();
	}
	else if (Ammoacount == 0)
	{
		AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

		if (PlayerController)
		{
			const FVector CameraLocation = PlayerController->FPSCameraComponent->GetComponentLocation();
			
			UGameplayStatics::PlaySoundAtLocation(this,DrySound,CameraLocation,
				1.0f,1.0f,0.0f);
		}
	}
}

void AAK_Weapon::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
FString::Printf(TEXT("Reloading...")));
		
	Mesh->PlayAnimation(ReloadAnimAsset, false);
	
	FTimerHandle ReloadTimerHandle;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::SetAmmoMax, ReloadTime, false);
}

void AAK_Weapon::AmmoCheck()
{
	AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	if (PlayerController)
	{
		if (!PlayerController->IsReloading)
		{
			Ammoacount--;
			Mesh->PlayAnimation(FireAnimAsset, false);
			
			const FVector CameraLocation = PlayerController->FPSCameraComponent->GetComponentLocation();
			const float RandomFireSound = FMath::RandRange(0.7f,1.2f);

			UGameplayStatics::PlaySoundAtLocation(this,FireSound,CameraLocation,
				RandomFireSound + 2.5f,
				RandomFireSound, 0.0f,AK_Attenuation,AK_Concurrency);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
FString::Printf(TEXT("Shooting!")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
FString::Printf(TEXT("Can't Shooting...")));
		}
	}
		
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		FString::Printf(TEXT("Ammoacount : %d"),Ammoacount));
}

void AAK_Weapon::LineTrace()
{
	AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	if (PlayerController)
	{
		FHitResult HitResult;

		float BulletSpreadRandom = FMath::RandRange(-1.0f, BulletSpread);
		
		const FVector Start = PlayerController->FPSCameraComponent->GetComponentLocation();
		const FVector ForwardVector = PlayerController->FPSCameraComponent->GetForwardVector();
		const FVector End = ((ForwardVector * 20000.0f) + Start + BulletSpreadRandom);
		
		if (GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECollisionChannel::ECC_Visibility))
		{
			// 판정 시스템
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,FString::Printf(TEXT("Hit! (Camera)")));
		}

		const FVector FireStart = FirePoint->GetComponentLocation();
		const FVector FireEnd = HitResult.ImpactPoint;
		
		if (GetWorld()->LineTraceSingleByChannel(HitResult,FireStart,FireEnd,ECollisionChannel::ECC_Visibility))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,FString::Printf(TEXT("Hit! (FirePoint)")));
		}
		
		SpawnDecalTracer(FireStart,FireEnd, FireEnd);
	}
}

void AAK_Weapon::SpawnDecalTracer(const FVector Location, const FVector SpawnTransformLocation, const FVector ImpactPoint)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,FString::Printf(TEXT("CAll of SpawnDecal (AK) ")));

	MuzzleFlashFunction();
	
	FActorSpawnParameters spawnWeaponParams;
	spawnWeaponParams.Owner = this;

	FRotator MakeXRot = UKismetMathLibrary::MakeRotFromX(ImpactPoint);

	const FName PathBulletDecal_BP = TEXT("Blueprint'/Game/PFSPawn/BulletDecal_BP.BulletDecal_BP_C'");
	UClass* GeneratedBulletDecal = Cast<UClass>(StaticLoadObject(UClass::StaticClass(),NULL,*PathBulletDecal_BP.ToString()));
	GetWorld()->SpawnActor<AActor>(GeneratedBulletDecal, SpawnTransformLocation, MakeXRot,spawnWeaponParams);
	
	if (FMath::RandRange(1,10) >= 0)
	{
		const AFPSCharacter* PlayerController = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		
		const FRotator CameraLocation = PlayerController->FPSCameraComponent->GetComponentRotation();
		const FTransform SpawnTL = UKismetMathLibrary::MakeTransform(Location,CameraLocation, FVector(0.1f,0.1f,0.1f));

		const FName PathTracerRound_BP = TEXT("Blueprint'/Game/PFSPawn/TracerRound_BP.TracerRound_BP_C'");
		UClass* GeneratedTracerRound = Cast<UClass>(StaticLoadObject(UClass::StaticClass(),NULL,*PathTracerRound_BP.ToString()));
		GetWorld()->SpawnActor<AActor>(GeneratedTracerRound, SpawnTL, spawnWeaponParams);
	}
}

void AAK_Weapon::MuzzleFlashFunction()
{
	PointLight->SetIntensity(40000.0f);
	MuzzleFlash->SetVisibility(true,false);
	
	FTimerHandle MuzzleFlashDelay;
	GetWorldTimerManager().SetTimer(MuzzleFlashDelay, this, &AAK_Weapon::MuzzleFlashEnd,0.1f, false);
}

void AAK_Weapon::MuzzleFlashEnd() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,FString::Printf(TEXT("CAll of MuzzleFlashEnd")));
	
	PointLight->SetIntensity(0.0f);
	MuzzleFlash->SetVisibility(false,false);

	float Rnd1 = FMath::RandRange(-90.0f,90.0f);
	MuzzleFlash->SetRelativeRotation(FRotator(Rnd1, 90.0f,90.0f));

	float Rnd2 = FMath::RandRange(0.2f,0.6f);
	MuzzleFlash->SetWorldScale3D(FVector(Rnd2,Rnd2,Rnd2));
}






