// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// COMPONENTS
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = MaxSpeed;
	FloatingPawnMovement->Acceleration = 3000.f;
	FloatingPawnMovement->Deceleration = 4000.f;

	CurrentInputVector = FVector::ZeroVector;

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// movement
	if (!CurrentInputVector.IsNearlyZero())
	{
		// Normalize to handle diagonal speed
		FVector MoveVector = CurrentInputVector.GetClampedToMaxSize(1.f);
		FloatingPawnMovement->AddInputVector(MoveVector);
	}


	// rotation
	FVector MoveInput = FVector(InputForward, InputRight, 0.f);

	if (!MoveInput.IsNearlyZero())
	{
		MoveInput.Normalize();
		FRotator TargetRotation = MoveInput.Rotation();
		SetActorRotation(TargetRotation);

		FloatingPawnMovement->AddInputVector(MoveInput * MaxSpeed * DeltaTime);
	}

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShip::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShip::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShip::Fire);
}


void AShip::MoveForward(float Value)
{
	CurrentInputVector.X = FMath::Clamp(Value, -1.f, 1.f);
	InputForward = Value;
}

void AShip::MoveRight(float Value)
{
	CurrentInputVector.Y = FMath::Clamp(Value, -1.f, 1.f);
	InputRight = Value;
}

void AShip::Fire()
{
	
}
