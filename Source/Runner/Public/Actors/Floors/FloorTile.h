#pragma once


#include "CoreMinimal.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;
class AObstacles;
class ACoinsItems;
class ABooster;

UCLASS()
class RUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()

public:
	AFloorTile();


	UFUNCTION() void ActivateBridge();
	UFUNCTION() void DesactivateBridge();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets)
	class UStaticMeshComponent* Bridge1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	TSubclassOf<AObstacles> SmallObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	TSubclassOf<AObstacles> SmallObstacleUpClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	TSubclassOf<AObstacles> BigObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	TSubclassOf<ACoinsItems> CoinClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	TSubclassOf<ABooster> BoosterClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* TileStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* TileEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPointCoinsLeftBack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coins")
	int CoinsLeftBackType = 0;
	/// type 0 : no obstacle
	/// type 1 : small obstacle
	/// type 2 : small obstacle up
	/// type 3 : big obstacle
	/// type 4 : void

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPointCoinsLeftFront;
	UPROPERTY(EditAnywhere , Category = "Coins")
	int CoinsLeftFrontType = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPointCoinsRightBack;
	UPROPERTY(EditAnywhere, Category = "Coins")
	int CoinsRightBackType = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPointCoinsRightFront;
	UPROPERTY(EditAnywhere,Category = "Coins")
	int CoinsRightFrontType = 0;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPointCoinsCenterBack;
	UPROPERTY(EditAnywhere,  Category = "Coins")
	int CoinsCenterBackType = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPointCoinsCenterFront;
	UPROPERTY(EditAnywhere,  Category = "Coins")
	int CoinsCenterFrontType = 0;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* CenterLane;
	UPROPERTY(EditAnywhere, Category = "Coins")
	int CoinsCenterLaneType = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* RightLane;
	UPROPERTY(EditAnywhere, Category = "Coins")
	int CoinsRightLaneType = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* LeftLane;
	UPROPERTY(EditAnywhere,  Category = "Coins")
	int CoinsLeftLaneType = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	int SpawnPercent1 = 70;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	int SpawnPercent2 = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	int SpawnPercent3 = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	TArray<AActor*> ObstaclesArray;




	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return AttachPoint->GetComponentTransform();
	}

	UFUNCTION(BlueprintCallable) void SpawnItems();
	UFUNCTION(BlueprintCallable) void SpawnCoinsItems(const int LastCoinLane);

	UFUNCTION() FVector GetNearestPointInLane(const int Lane, const FVector position);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	class AGameModeRunner* RunGameMode;

	UPROPERTY()
	FTimerHandle DestroyHandle;

	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyFloorTile();

	UFUNCTION()
	void SpawnLaneItem(UArrowComponent* Lane,int currentRelatedCoinPosition);

private:

};
