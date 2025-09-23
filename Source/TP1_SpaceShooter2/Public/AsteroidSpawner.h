// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

class AAsteroid;

UCLASS()
class TP1_SPACESHOOTER2_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnAsteroid();

	FVector GetSpawnLocationOnEdge();

	FTimerHandle SpawnTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AsteroidClass;

	UPROPERTY(EditAnywhere)
	float SpawnInterval = 2.f;

	UPROPERTY(EditAnywhere)
	FVector SpawnAreaMin;

	UPROPERTY(EditAnywhere)
	FVector SpawnAreaMax;

};
