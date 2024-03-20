// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "GridSpawn.h"
#include "SlowDownBonus.generated.h"

UCLASS()
class SNAKEGAME_API ASlowDownBonus : public AActor, public IInteractable, public IGridSpawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlowDownBonus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	virtual void SpawnAtGridLocation(const FVector& Location) override;
};
