// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

#include "Asteroid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AAsteroidSpawner::SpawnAsteroid, SpawnInterval, true);
	
}

// Called every frame
void AAsteroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAsteroidSpawner::SpawnAsteroid()
{
	if (!AsteroidClass) return;

	FVector SpawnLocation = GetSpawnLocationOnEdge();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters Params;
	Params.Owner = this;

	AAsteroid* NewAsteroid = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass, SpawnLocation, SpawnRotation, Params);
	if (NewAsteroid)
	{
		// Récupérer la position du joueur
		AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Player)
		{
			FVector PlayerLocation = Player->GetActorLocation();
			NewAsteroid->InitMovement(SpawnLocation, PlayerLocation);
		}

		// Random ccale
		float Scale = FMath::FRandRange(1.5f, 5.0f); 
		NewAsteroid->SetActorScale3D(FVector(Scale));

		// Random rotation
		FRotator RandomRot = FRotator(
	   FMath::FRandRange(0.f, 360.f),
	   FMath::FRandRange(0.f, 360.f),
	   FMath::FRandRange(0.f, 360.f)
   );
		NewAsteroid->SetActorRotation(RandomRot);

		// Vitesse de rotation (qu’on utilisera dans Tick)
		NewAsteroid->SetRotationSpeed(FMath::FRandRange(30.f, 120.f));
		
	}
}


FVector AAsteroidSpawner::GetSpawnLocationOnEdge()
{
	// Choisir un bord aléatoire : 0 = gauche, 1 = droite, 2 = bas, 3 = haut
	int32 Side = FMath::RandRange(0, 3);

	float X, Y, Z = 0.f;

	switch (Side)
	{
	case 0: // gauche
		X = SpawnAreaMin.X;
		Y = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
		break;

	case 1: // droite
		X = SpawnAreaMax.X;
		Y = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
		break;

	case 2: // bas
		X = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
		Y = SpawnAreaMin.Y;
		break;

	case 3: // haut
		X = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
		Y = SpawnAreaMax.Y;
		break;
	}
	return FVector(X, Y, Z);
}

