// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridSpawn.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGridSpawn : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SNAKEGAME_API IGridSpawn
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SpawnAtGridLocation(const FVector& Location) = 0;
};
