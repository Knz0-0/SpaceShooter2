// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class TP1_SPACESHOOTER2_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	
	// COMPONENTS
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

	// VARIABLES
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxSpeed = 1200.f;


	// FUNCTIONS
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Fire();

private:

	FVector CurrentInputVector;
	float InputForward = 0.f;
	float InputRight = 0.f;


};
