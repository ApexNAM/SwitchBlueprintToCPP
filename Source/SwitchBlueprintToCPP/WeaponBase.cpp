// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 무기 메쉬 컴포넌트 생성 
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	// 무기 그림자 비활성화
	Mesh->bCastDynamicShadow = false;
	Mesh->CastShadow = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 발사 함수
void AWeaponBase::Fire()
{

}

// 재장전 함수
void AWeaponBase::Reload()
{
	
}

void AWeaponBase::SetAmmoMax() 
{
	Ammoacount = MaxAmmo;
}



