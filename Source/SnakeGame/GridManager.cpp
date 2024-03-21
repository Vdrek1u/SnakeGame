// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Interactable.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::GenerateGrid()
{
    FVector GridOrigin = GetActorLocation();

    GridPoints.Empty();
    for (int32 x = 0; x < GridSizeX; ++x)
    {
        for (int32 y = 0; y < GridSizeY; ++y)
        {
            FVector PointLocation = GridOrigin + FVector(x * CellSize, y * CellSize, 0.f);
            GridPoints.Add(PointLocation);

            if (bVisualizeGrid)
            {
                 DrawDebugPoint(GetWorld(), PointLocation, 5.0f, FColor::Black , true, -1.0f, 0);
            }

        }
    }
}

bool AGridManager::IsCellOccupied(const FVector& Location) const
{
    FVector BoxExtent = FVector(SizeOfCell / 2.0f, SizeOfCell / 2.0f, SizeOfCell / 2.0f);
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(BoxExtent);

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams QueryParams;
    QueryParams.bReturnPhysicalMaterial = false;
    QueryParams.AddIgnoredActor(this);

    bool bOverlapFound = GetWorld()->OverlapMultiByChannel(Overlaps, Location, FQuat::Identity, ECC_GameTraceChannel1, CollisionShape, QueryParams);

    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* OverlappedActor = Result.GetActor();
        if (OverlappedActor && OverlappedActor->Implements<UInteractable>())
        {
            return true; // Найден актор, реализующий IInteractable
        }
    }

    return false; // Клетка не занята
}

bool AGridManager::SpawnObjectAtRandomLocation(TSubclassOf<AActor> ObjectToSpawn)
{
    if (!GetWorld()) return false;

    for (int32 Try = 0; Try < 15; ++Try)
    {
        int32 RandomIndex = FMath::RandRange(0, GridPoints.Num() - 1);
        FVector SpawnLocation = GridPoints[RandomIndex];

        if (!IsCellOccupied(SpawnLocation))
        {
            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ObjectToSpawn, FTransform(FRotator::ZeroRotator, SpawnLocation));
            IGridSpawn* Spawnable = Cast<IGridSpawn>(SpawnedActor);
            if (Spawnable)
            {
                Spawnable->SpawnAtGridLocation(SpawnLocation);
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Spawn at %s"), *SpawnLocation.ToString()));
                }
               return true;
            }
            else if (SpawnedActor)
            {
                SpawnedActor->Destroy();
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spawnable odject haven't interf %s"), *SpawnLocation.ToString()));
                }
            }
        }
        else
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("SpawnPoint is occupated %s"), *SpawnLocation.ToString()));
            }
        }
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spawn failed after 15 try"));
    }
    return false;
}
