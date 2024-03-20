// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

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

    // Настройка параметров запроса
    FCollisionQueryParams QueryParams;
    QueryParams.bReturnPhysicalMaterial = false;
    QueryParams.AddIgnoredActor(this); 

    // Выполнение проверки пересечения
    bool bIsOccupiedS = GetWorld()->OverlapBlockingTestByChannel(
        Location,
        FQuat::Identity,
        ECC_WorldStatic,
        CollisionShape,
        QueryParams
    );
    bool bIsOccupiedD = GetWorld()->OverlapBlockingTestByChannel(
        Location,
        FQuat::Identity,
        ECC_WorldDynamic,
        CollisionShape,
        QueryParams
    );

    return bIsOccupiedS || bIsOccupiedD;
}

bool AGridManager::SpawnObjectAtRandomLocation(TSubclassOf<AActor> ObjectToSpawn)
{
    if (!GetWorld()) return false;

    for (int32 Try = 0; Try < 100; ++Try)
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
               return true;
            }
            else if (SpawnedActor)
            {
                SpawnedActor->Destroy();
            }
        }
    }

    return false;


}


