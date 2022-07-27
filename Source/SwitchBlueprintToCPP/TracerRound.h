// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TracerRound.generated.h"


UCLASS()
class SWITCHBLUEPRINTTOCPP_API ATracerRound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracerRound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, Category="Bullet Object")
	UStaticMeshComponent* BulletObject;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
