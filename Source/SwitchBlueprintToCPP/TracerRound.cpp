// Fill out your copyright notice in the Description page of Project Settings.


#include "TracerRound.h"

// Sets default values
ATracerRound::ATracerRound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->OnComponentHit.AddDynamic(this,&ATracerRound::OnHit);
	
	BulletObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletObject"));
	BulletObject->SetupAttachment(Sphere);
	BulletObject->OnComponentHit.AddDynamic(this,&ATracerRound::OnHit);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	ProjectileMovement->InitialSpeed = 7000.0f;
	ProjectileMovement->Bounciness = 0.0f;
	ProjectileMovement->Friction = 0.0f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 5.0f;
	ProjectileMovement->InterpLocationTime = 0.05f;
}

// Called when the game starts or when spawned
void ATracerRound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATracerRound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATracerRound::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		Destroy();
	}
}


