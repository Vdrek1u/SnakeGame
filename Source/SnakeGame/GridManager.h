// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpawn.h"
#include "GridManager.generated.h"

UCLASS()
class SNAKEGAME_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridSizeX = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridSizeY = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bVisualizeGrid = false;
	UPROPERTY()
	TArray<FVector> GridPoints;
	UPROPERTY()
	float SizeOfCell;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void GenerateGrid();
	UFUNCTION()
	bool SpawnObjectAtRandomLocation(TSubclassOf<AActor> ObjectToSpawn);
	UFUNCTION()
	bool IsCellOccupied(const FVector& Location) const;

};
