// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "SnakeBase.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->AddSnakeElement();
			Snake->SpeedUp();
			

			float SpawnChance = 0.2;
			if (FMath::RandRange(0.f, 1.f) < SpawnChance)
			{
				Snake->SpawnSlowDownBonus();
			}

			Destroy();
		}
	}
}

void AFood::SpawnAtGridLocation(const FVector& Location)
{
	SetActorLocation(Location);
}

