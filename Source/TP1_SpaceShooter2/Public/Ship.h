// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, MaxHealth, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, Score);

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

	// --- Health ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MaxHealth = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
	float CurrentHealth = 5;

	// Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FOnScoreChanged OnScoreChanged;

	// --- Score ---
	UPROPERTY(BlueprintReadOnly, Category="Stats")
	int32 Score = 0;


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	
	// COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

	// VARIABLES
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship")
	bool bCanTakeDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship")
	float InvulnerabilityTime = 1.0f; // secondes

	UPROPERTY(EditAnywhere, Category="Combat")
	TSubclassOf<class AProjectile> ProjectileClass;


	// FUNCTIONS
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Fire();
	
	UFUNCTION()
	void OnAsteroidOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void LoseLife();
	void ResetInvulnerability();
	void AddScore(int32 Ammount);
	void AddScorePerSecond();


private:

	FVector CurrentInputVector;
	float InputForward = 0.f;
	float InputRight = 0.f;
	
	FTimerHandle ScoreTimerHandle;
	FTimerHandle InvulnerabilityTimerHandle;


};
