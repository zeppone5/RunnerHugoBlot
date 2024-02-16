#include "GameMode/GameModeRunner.h"
#include "../../../../Source/Runner/Public/Actors/Floors/FloorTile.h"
#include "Kismet/GameplayStatics.h"




void AGameModeRunner::BeginPlay()
{
	GenerationArg1 = FMath::RandRange(1, 10);
GenerationArg2 = FMath::RandRange(1, 10);
GenerationArg3 = FMath::RandRange(1, 10);
GenerationArg4 = FMath::RandRange(1, 10);
	CreateInitialFloorTiles();
}


void AGameModeRunner::CreateInitialFloorTiles()
{
	AFloorTile* Tile = AddFloorTile(false);

	if (Tile)
	{
		LaneSwitchValues.Add(FVector2f(Tile->LeftLane->GetComponentLocation().X, Tile->LeftLane->GetComponentLocation().Y));
		LaneSwitchValues.Add(FVector2f(Tile->CenterLane->GetComponentLocation().X, Tile->CenterLane->GetComponentLocation().Y));
		LaneSwitchValues.Add(FVector2f(Tile->RightLane->GetComponentLocation().X, Tile->RightLane->GetComponentLocation().Y));
	}

	//for (float Val : LaneSwitchValues)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("LANE VALUE: %f"), Val);
	//}

	AddFloorTile(false);
	AddFloorTile(false);


	for (int i = 0; i < NumItinitalFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* AGameModeRunner::AddFloorTile(const bool bSpawnItems, const int LastCoinLane)
{
	UWorld* World = GetWorld();

	if (World)
	{
		//sin and cos based generation more smooth than random
		float r =
			GenerationArg3 * FMath::Sin(0.01f * PlateformNumber) +
			GenerationArg1 * FMath::Cos(0.03f * PlateformNumber) +
			GenerationArg4 * FMath::Sin(0.5f * PlateformNumber) +
			GenerationArg2 * FMath::Cos(0.7f * PlateformNumber);
		TSubclassOf<AFloorTile> currentTileClass = r > 0 ? FloorTileClass : FloorTileClassVoid;


		AFloorTile* Tile;
		//avoid cliping with this simple trick !
		if (tileAlternator) {
			NextSpawnPoint.SetLocation(NextSpawnPoint.GetLocation() + FVector(0, 0, 2.f));
			Tile = World->SpawnActor<AFloorTile>(currentTileClass, NextSpawnPoint);
			tileAlternator = false;
		}
		else {
			NextSpawnPoint.SetLocation(NextSpawnPoint.GetLocation() + FVector(0, 0, -2.f));
			Tile = World->SpawnActor<AFloorTile>(currentTileClass, NextSpawnPoint);

			tileAlternator = true;
		}

		if (Tile)
		{
			if (bSpawnItems)
			{
				Tile->SpawnItems();
				Tile->SpawnCoinsItems(LastCoinLane);
			}
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		FloorTilesArray.Add(Tile);
		return Tile;
	}
	return nullptr;
}

FRotator AGameModeRunner::GenerateNextRotation()
{
	FRotator NextRotation;
	//based on this formula (:
	//a Sin[0.02x] + b Cos[ 0.03x] + c Sin[0.04x] + d Cos[ 0.05x]

	float yaw =
		GenerationArg1 * FMath::Sin(0.1f * PlateformNumber) +
		GenerationArg2 * FMath::Cos(0.3f * PlateformNumber) +
		GenerationArg3 * FMath::Sin(0.5f * PlateformNumber) +
		GenerationArg4 * FMath::Cos(0.7f * PlateformNumber);


	PlateformNumber += 1;
	return FRotator(0,yaw , 0);
}


void AGameModeRunner::AddCoin()
{
	TotalCoins += 1;

	UE_LOG(LogTemp, Warning, TEXT("TOTAL COINS: %d"), TotalCoins);
}


