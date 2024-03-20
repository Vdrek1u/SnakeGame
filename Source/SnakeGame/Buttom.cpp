// Fill out your copyright notice in the Description page of Project Settings.


#include "Buttom.h"
#include "Components/StaticMeshComponent.h"
#include "SnakeBase.h"

// Sets default values
AButtom::AButtom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

}

// Called when the game starts or when spawned
void AButtom::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AButtom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtom::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake) && Snake->GetSnakeLength() >= 5)
		{
			ButtomActiveted();
		}
	}
}

void AButtom::ButtomActiveted()
{
	if (!MeshComponent) return;

	if (!DynamicMaterialInstance)
	{
		DynamicMaterialInstance = MeshComponent->CreateDynamicMaterialInstance(0);
	}

	if (DynamicMaterialInstance)
	{
		FLinearColor NewColor = FLinearColor::Red;
		DynamicMaterialInstance->SetVectorParameterValue(FName("DynamicColor"), NewColor);
	}
}