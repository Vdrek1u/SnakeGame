// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Food.h"
#include "GridManager.h"
#include "Interactable.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 0.5f;
	SpeedBonus = 0.5f;
	LastMoveDirection = EMovementDirection::DOWN;
	bHasDirectionChanged = false;

	static ConstructorHelpers::FClassFinder<AActor> FoodBP(TEXT("/Game/InteractOBJ/FoodBP"));
	if (FoodBP.Succeeded())
	{
		BP_Food_Class = FoodBP.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> SlowDownBonusBP(TEXT("/Game/InteractOBJ/SlowDownBonusBP"));
	if (SlowDownBonusBP.Succeeded())
	{
		BP_SlowDown_Class = SlowDownBonusBP.Class;
	}
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(1);
	GetWorld()->GetTimerManager().SetTimer(SpawnFoodTimerHandle, this, &ASnakeBase::SpawnFood, 0.1f, true, 1.0f);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("GridManagerInstance"), FoundActors);

	if (FoundActors.Num() > 0)
	{
		AGridManager* FoundGridManager = Cast<AGridManager>(FoundActors[0]);
		if (FoundGridManager)
		{
			GridManager = FoundGridManager;
			GridManager->GenerateGrid();
			for (int i = 0; i < 5; ++i)
			{
				SpawnFood();
			}
		}
	}
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsWaitingForInput)
	{
		bHasDirectionChanged = false;
		Move();
	}
}

void ASnakeBase::ChangeDirection(EMovementDirection NewDirection)
{
	if (bIsWaitingForInput)
	{
		if (NewDirection != LastMoveDirection)
		{
			bIsWaitingForInput = false;
			LastMoveDirection = NewDirection;
			bHasDirectionChanged = true;
		}
	}
	
	else if (!bHasDirectionChanged)
	{
		if ((NewDirection == EMovementDirection::UP && LastMoveDirection != EMovementDirection::DOWN) ||
			(NewDirection == EMovementDirection::DOWN && LastMoveDirection != EMovementDirection::UP) ||
			(NewDirection == EMovementDirection::LEFT && LastMoveDirection != EMovementDirection::RIGHT) ||
			(NewDirection == EMovementDirection::RIGHT && LastMoveDirection != EMovementDirection::LEFT))
		{
			LastMoveDirection = NewDirection;
			bHasDirectionChanged = true;
		}
	}
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	FVector NewLocation = FVector::ZeroVector;

	if (SnakeElements.Num() > 0)
	{
		ASnakeElementBase* LastElement = SnakeElements.Last();
		FVector LastElementLocation = LastElement->GetActorLocation();

		switch (LastMoveDirection)
		{
		case EMovementDirection::UP:
			NewLocation = LastElementLocation + FVector(-ElementSize, 0, 0);
			break;
		case EMovementDirection::DOWN:
			NewLocation = LastElementLocation + FVector(ElementSize, 0, 0);
			break;
		case EMovementDirection::LEFT:
			NewLocation = LastElementLocation + FVector(0, -ElementSize, 0);
			break;
		case EMovementDirection::RIGHT:
			NewLocation = LastElementLocation + FVector(0, ElementSize, 0);
			break;
		}
	}

	for (int i = 0; i < ElementsNum; ++i)
	{
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		
			NewSnakeElement->SnakeOwner = this;
			SnakeElements.Add(NewSnakeElement);

			if (i == 0 && SnakeElements.Num() == 1)
			{
				NewSnakeElement->SetFirstElementType();
			}
	}

	/*for (int i = 0; i < ElementsNum; ++i)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);

		ASnakeElementBase* NewSnakeElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElement->SnakeOwner = this;
		int32 ElementIndex = SnakeElements.Add(NewSnakeElement);
		if (ElementIndex == 0)
		{
			NewSnakeElement->SetFirstElementType();
		}
	}*/
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	MovementSpeed = ElementSize;

	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += MovementSpeed;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= MovementSpeed;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += MovementSpeed;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= MovementSpeed;
		break;
	}

	//AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PreviousElement = SnakeElements[i - 1];
		FVector PreviosLocation = PreviousElement->GetActorLocation();
		CurrentElement->SetActorLocation(PreviosLocation);
	}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirst);
		}
	}
	if (Other && Other != this)
	{
		if (Other)
		{
			FString Message = FString::Printf(TEXT("Overlap with %s"), *Other->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Message);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}
}

void ASnakeBase::SpawnFood()
{
	if (GridManager)
	{
		GridManager->SpawnObjectAtRandomLocation(BP_Food_Class);
	}
}

void ASnakeBase::SpawnSlowDownBonus()
{
	if (GridManager)
	{
		GridManager->SpawnObjectAtRandomLocation(BP_SlowDown_Class);
	}
}

void ASnakeBase::SpeedUp()
{
	if (SpeedBonus > 0.2)
	{
		SetActorTickInterval(SpeedBonus -= 0.02);
	}
}

void ASnakeBase::SpeedDown()
{
	if (SpeedBonus < 0.4)
	{
		SetActorTickInterval(SpeedBonus = 0.4);
	}

	else
	{
		SetActorTickInterval(SpeedBonus = 0.5);
	}
}
