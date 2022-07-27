// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletDecal.generated.h"

UCLASS()
class SWITCHBLUEPRINTTOCPP_API ABulletDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletDecal();
	
public:
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere)
	UMaterial* BulletDecalMat;
};
