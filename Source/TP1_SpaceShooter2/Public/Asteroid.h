// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class TP1_SPACESHOOTER2_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float RotationSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Asteroid properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetOffset = FVector::ZeroVector; // Permet de viser légèrement à côté du joueur

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AsteroidHealth;

	UPROPERTY()
	UMaterialInstanceDynamic* AsteroidMaterial;

	// Init movement
	void InitMovement(FVector SpawnLocation, FVector PlayerLocation);
	void SetRotationSpeed(float Speed);

	// Functions
	void LoseHealth();
};
