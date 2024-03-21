// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Buttom.generated.h"

UCLASS()
class SNAKEGAME_API AButtom : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtom();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UMaterialInstanceDynamic* DynamicMaterialInstance = nullptr;

	UPROPERTY()
	bool bIsButtomActivated = false;

	UPROPERTY(EditAnywhere, Category = "Setup")
	AActor* DoorPart1;

	UPROPERTY(EditAnywhere, Category = "Setup")
	AActor* DoorPart2;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MoveDistance = 120.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	void ButtomActiveted();

};
