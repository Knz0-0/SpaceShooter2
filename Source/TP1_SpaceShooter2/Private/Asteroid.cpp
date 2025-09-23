// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// COMPONENTS
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Projectile movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;


}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotationSpeed != 0.f)
	{
		AddActorLocalRotation(FRotator(RotationSpeed * DeltaTime, RotationSpeed * DeltaTime, RotationSpeed * DeltaTime));
	}


	// destroy if needed
	if (GetActorLocation().X > 2500 || GetActorLocation().X < -2500 || GetActorLocation().Y > 2500 || GetActorLocation().Y < -2500)
	{
		Destroy();
	}

}


void AAsteroid::InitMovement(FVector SpawnLocation, FVector PlayerLocation)
{
	SetActorLocation(SpawnLocation);

	// Viser le joueur avec un offset aléatoire
	FVector Direction = (PlayerLocation + TargetOffset - SpawnLocation).GetSafeNormal();

	// Vitesse aléatoire
	float Speed = FMath::RandRange(MinSpeed, MaxSpeed);
	ProjectileMovementComponent->Velocity = Direction * Speed;
}

void AAsteroid::SetRotationSpeed(float Speed)
{
	RotationSpeed = Speed;
}

