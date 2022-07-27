// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletDecal.h"

#include "Components/DecalComponent.h"

// Sets default values
ABulletDecal::ABulletDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	static ConstructorHelpers::FObjectFinder<UMaterial> FBulletDecalMat(TEXT("Material'/Game/PFSPawn/BulletDecal_MAT'"));

	if (FBulletDecalMat.Succeeded())
	{
		Decal->SetDecalMaterial(FBulletDecalMat.Object);
	}

	Decal->FadeStartDelay = 5.0f;
	Decal->FadeDuration = 5.0f;
}

