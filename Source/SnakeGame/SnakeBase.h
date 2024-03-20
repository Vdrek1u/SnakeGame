// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.h"
#include "SnakeBase.generated.h"

class ASnakeElementBase;

UENUM()
enum class EMovementDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};


UCLASS()
class SNAKEGAME_API ASnakeBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeElementBase> SnakeElementClass;

	UPROPERTY(EditDefaultsOnly)
	float ElementSize;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly)
	float SpeedBonus;

	UPROPERTY()
	TArray<ASnakeElementBase*> SnakeElements;

	UPROPERTY()
	EMovementDirection LastMoveDirection;

	UPROPERTY()
	bool bHasDirectionChanged = false;

	UPROPERTY(EditAnywhere)
	AGridManager* GridManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BP_Food_Class;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus")
	TSubclassOf<AActor> BP_SlowDown_Class;

	UPROPERTY()
	FTimerHandle SpawnFoodTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddSnakeElement(int ElementsNum = 1);
	UFUNCTION()
	void Move();
	UFUNCTION()
	void SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other);
	UFUNCTION()
	void ChangeDirection(EMovementDirection NewDirection);
	UFUNCTION()
	void SpawnFood();
	UFUNCTION()
	void SpawnSlowDownBonus();
	UFUNCTION()
	void SpeedUp();
	UFUNCTION()
	void SpeedDown();
	UFUNCTION()
	int32 GetSnakeLength() const { return SnakeElements.Num(); }
};