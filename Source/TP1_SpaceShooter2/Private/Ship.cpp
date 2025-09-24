// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "CoreMinimal.h"
#include "Asteroid.h"
#include "Projectile.h"
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
	// Verrouille les rotations physiques sur X et Y
    BoxComponent->BodyInstance.bLockXRotation = true;
    BoxComponent->BodyInstance.bLockYRotation = true;
    BoxComponent->BodyInstance.bLockZRotation = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);
	StaticMesh->SetSimulatePhysics(false);
    StaticMesh->SetEnableGravity(false);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = MaxSpeed;
	FloatingPawnMovement->Acceleration = 1000.f;
	FloatingPawnMovement->Deceleration = 1000.f;

	FloatingPawnMovement->SetPlaneConstraintEnabled(true);
	FloatingPawnMovement->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));
	FloatingPawnMovement->SnapUpdatedComponentToPlane();

	
	CurrentInputVector = FVector::ZeroVector;

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	//CurrentLives = MaxLives;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AShip::OnAsteroidOverlap);
	GetWorld()->GetTimerManager().SetTimer(ScoreTimerHandle, this, &AShip::AddScorePerSecond, 1.0, true);
	
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
	if (ProjectileClass) // tu définis ProjectileClass = TSubclassOf<AProjectile> exposé dans ton .h
	{
		FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * 100.f; 
		FRotator MuzzleRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	}

	GEngine->AddOnScreenDebugMessage(
			-1,                 // Key: Unique integer key to prevent duplicate messages. -1 means no key, so it won't prevent duplicates.
			1.0f,               // TimeToDisplay: How long the message will be displayed in seconds.
			FColor::Yellow,     // DisplayColor: The color of the text.
			TEXT("FIRE!") // DebugMessage: The message to display.
		);}



void AShip::OnAsteroidOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AAsteroid* Asteroid = Cast<AAsteroid>(OtherActor);
	if (Asteroid)
	{
		// Calcul direction opposée
		FVector PushDirection = (GetActorLocation() - Asteroid->GetActorLocation()).GetSafeNormal();
		float PushStrength = 2000.f;

		// Déplacer le ship en conséquence
		AddActorWorldOffset(PushDirection * PushStrength * GetWorld()->GetDeltaSeconds(), true);
		if (bCanTakeDamage)
		{
			LoseLife();
		}
		
	}

	
}

void AShip::LoseLife()
{
	CurrentLives--;
	bCanTakeDamage = false;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Max Lives: %d"), MaxLives));
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Current Lives: %d"), CurrentLives));


	// On notifie les listeners (HUD, UI, etc.)
	OnHealthChanged.Broadcast(MaxLives, CurrentLives);

	
	// Invulnerability
	GetWorld()->GetTimerManager().SetTimer(
		InvulnerabilityTimerHandle,
		this,
		&AShip::ResetInvulnerability,
		InvulnerabilityTime,
		false
	);
}

void AShip::ResetInvulnerability()
{
	bCanTakeDamage = true;
}

void AShip::AddScore(int32 Amount)
{
	Score += Amount;
}

void AShip::AddScorePerSecond()
{
	AddScore(10);
}

