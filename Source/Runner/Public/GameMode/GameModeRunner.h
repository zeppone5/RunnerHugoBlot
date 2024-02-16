#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeRunner.generated.h"

class AFloorTile;

UCLASS()
class RUNNER_API AGameModeRunner : public AGameModeBase
{
	GENERATED_BODY()

public:


	TArray<AFloorTile*> FloorTilesArray;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AFloorTile> FloorTileClassVoid;
	UPROPERTY(BlueprintReadWrite)	TArray<AActor*> Coins;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 NumItinitalFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<FVector2f> LaneSwitchValues;

	UPROPERTY(VisibleAnywhere)
	int32 TotalCoins = 0;

	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)
	AFloorTile* AddFloorTile(const bool bSpawnItems,const int LastCoinLane = 1);

	UFUNCTION() FRotator GenerateNextRotation();


	UFUNCTION(BlueprintCallable)
	void AddCoin();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, Category = "Generation")
	int PlateformNumber = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "Generation")
	float GenerationArg1 = 4, GenerationArg2 = 3, GenerationArg3 = 3, GenerationArg4 = 2;

private:
	bool tileAlternator = false;
};
