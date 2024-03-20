// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowDownBonus.h"
#include "SnakeBase.h"

// Sets default values
ASlowDownBonus::ASlowDownBonus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlowDownBonus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlowDownBonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlowDownBonus::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->AddSnakeElement(3);
			Snake->SpeedDown();
			Destroy();
		}
	}
}

void ASlowDownBonus::SpawnAtGridLocation(const FVector& Location)
{
	SetActorLocation(Location);
}

