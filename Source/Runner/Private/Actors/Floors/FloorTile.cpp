#include "Actors/Floors/FloorTile.h"
#include "GameFramework/Actor.h"
#include "../../../../../Source/Runner/Public/Charaters/RunCharacter.h"
#include "../../../../../Source/Runner/Public/GameMode/GameModeRunner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "../../../Public/Obstacles/Obstacles.h"
#include "../../../Public/Actors/Booster.h"
#include "../../../Public/Coins/CoinsItems.h"

// Sets default values
AFloorTile::AFloorTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	TileStart = CreateDefaultSubobject<UArrowComponent>(TEXT("Tile Start"));
	TileStart->SetupAttachment(SceneComponent);

	TileEnd = CreateDefaultSubobject<UArrowComponent>(TEXT("Tile End"));
	TileEnd->SetupAttachment(SceneComponent);


	Bridge1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Bridge1->SetupAttachment(SceneComponent);

	AttachPointCoinsLeftBack = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point Coins Left Back"));
	AttachPointCoinsLeftBack->SetupAttachment(SceneComponent);


	AttachPointCoinsLeftFront = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point Coins Left Front"));
	AttachPointCoinsLeftFront->SetupAttachment(SceneComponent);

	AttachPointCoinsRightBack = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point Coins Right Back"));
	AttachPointCoinsRightBack->SetupAttachment(SceneComponent);

	AttachPointCoinsRightFront = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point Coins Right Front"));
	AttachPointCoinsRightFront->SetupAttachment(SceneComponent);

	AttachPointCoinsCenterBack = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point Coins Center Back"));
	AttachPointCoinsCenterBack->SetupAttachment(SceneComponent);

	AttachPointCoinsCenterFront = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point Coins Center Front"));
	AttachPointCoinsCenterFront->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Point"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Point"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Point"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AFloorTile::ActivateBridge()
{
	Bridge1->SetVisibility(true);
	Bridge1->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AFloorTile::DesactivateBridge()
{
	Bridge1->SetVisibility(false);
	Bridge1->SetCollisionProfileName(TEXT("NoCollision"));
}

void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<AGameModeRunner>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);

	AttachPoint->SetRelativeRotation(RunGameMode->GenerateNextRotation());

	Bridge1->SetVisibility(false);
	Bridge1->SetCollisionProfileName(TEXT("NoCollision"));
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(SmallObstacleUpClass) && IsValid(BigObstacleClass) && IsValid(CoinClass) && IsValid(BoosterClass))
	{
		int RandVal = FMath::RandRange(0, 100);
		int lane1 = 0;
		if (RandVal <= SpawnPercent1)
		{
			lane1 = FMath::RandRange(0, 2);
			switch (lane1)
			{
			case 0:
				if (CoinsLeftLaneType != -1)
					SpawnLaneItem(LeftLane, lane1);
				break;
			case 1:
				SpawnLaneItem(CenterLane, lane1);
				break;
			case 2:
				if (CoinsRightLaneType != -1)
					SpawnLaneItem(RightLane, lane1);
				break;
			}
		}

		RandVal = FMath::RandRange(0, 100);
		int lane2 = 0;
		if (RandVal <= SpawnPercent2)
		{
			do {
				//monkey sort but it's ok (:
				lane2 = FMath::RandRange(0, 2);
			} while (lane2 == lane1);
			switch (lane2)
			{
			case 0:
				if (CoinsLeftLaneType != -1)
				SpawnLaneItem(LeftLane, lane2);
				break;
			case 1:
				SpawnLaneItem(CenterLane, lane2);
				break;
			case 2:
				if (CoinsLeftLaneType != -1)
				SpawnLaneItem(RightLane, lane2);
				break;
			}
		}

		RandVal = FMath::RandRange(0, 100);
		if (RandVal <= SpawnPercent3)
		{
			//split between hating and loving this
			int lane3 = (lane2 == 0) ? ((lane1 == 1) ? 2 : 1) : ((lane2 == 1) ? ((lane1 == 0) ? 2 : 0) : ((lane1 == 0) ? 1 : 0));
			switch (lane3)
			{
			case 0:
				if (CoinsLeftLaneType != -1)
				SpawnLaneItem(LeftLane, lane3);
				break;
			case 1:
				SpawnLaneItem(CenterLane, lane3);
				break;
			case 2:
				if (CoinsLeftLaneType != -1)
				SpawnLaneItem(RightLane, lane3);
				break;
			}
		}
	}
}
int _NewCoinLAne(const int LastCoinLane) {
	int RandVal = FMath::RandRange(0, 100);
	if (RandVal > 30)
	{
		return LastCoinLane;
	}
	else
	{
		switch (LastCoinLane)
		{
		case 0:
			FMath::RandBool() ? RandVal = 1 : RandVal = 2;
			return RandVal;
			break;
		case 1:
			FMath::RandBool() ? RandVal = 0 : RandVal = 2;
			return RandVal;
			break;
		case 2:
			FMath::RandBool() ? RandVal = 0 : RandVal = 1;
			return RandVal;
			break;
		default:
			break;
		}
	}
	return LastCoinLane;
}

void AFloorTile::SpawnCoinsItems(const int LastCoinLane)
{
	if (IsValid(CoinClass))
	{
		//////////////////////////////////////////////////////////////////////////First Coin//////////////////////////////////////////////////////////////////////////
		ACoinsItems* Coin1 = nullptr;
		int currentLane = _NewCoinLAne(LastCoinLane);
		switch (currentLane)
		{
		case 0:
			if (CoinsLeftBackType != -1)
				Coin1 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsLeftBack->GetComponentTransform());
			break;
		case 1:
			if (CoinsCenterBackType != -1)
				Coin1 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsCenterBack->GetComponentTransform());
			break;
		case 2:
			if (CoinsRightBackType != -1)
				Coin1 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsRightBack->GetComponentTransform());
			break;
		default:
			if (CoinsCenterBackType != -1)
				Coin1 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsCenterBack->GetComponentTransform());
			break;	

		}
		if (Coin1 != nullptr)
			ObstaclesArray.Add(Coin1);
		//////////////////////////////////////////////////////////////////////////Second Coin//////////////////////////////////////////////////////////////////////////
		ACoinsItems* Coin2;
		currentLane = _NewCoinLAne(LastCoinLane);
		FTransform SpawnTransform;
		float z;
		//I'm starting to hate switchs
		switch (currentLane)
		{
		case 0:
			SpawnTransform = LeftLane->GetComponentTransform();
			switch (CoinsLeftLaneType)
			{
			case 0:
				z = SpawnTransform.GetLocation().Z + 50;
				break;
			case 1:
				z = SpawnTransform.GetLocation().Z + 200;
				break;
			case 2:
				z = SpawnTransform.GetLocation().Z;
				break;
			case 3:
			case 4:
			case -1:
				z = -10000000;
				break;
			}
			break;
		case 1:
			SpawnTransform = CenterLane->GetComponentTransform();
			switch (CoinsCenterLaneType)
			{
			case 0:
				z = SpawnTransform.GetLocation().Z + 50;
				break;
			case 1:
				z = SpawnTransform.GetLocation().Z + 200;
				break;
			case 2:
				z = SpawnTransform.GetLocation().Z;
				break;
			case 3:
			case 4:
			case -1:
				z = -10000000;
				break;
			}
			break;
		case 2:
			SpawnTransform = RightLane->GetComponentTransform();
			switch (CoinsRightLaneType)
			{
			case 0:
				z = SpawnTransform.GetLocation().Z + 50;
				break;
			case 1:
				z = SpawnTransform.GetLocation().Z + 200;
				break;
			case 2:
				z = SpawnTransform.GetLocation().Z;
				break;
			case 3:
			case 4:
			case -1:
				z = -10000000;
				break;
			}
			break;
		}
		SpawnTransform.SetLocation(FVector(SpawnTransform.GetLocation().X, SpawnTransform.GetLocation().Y, z));
		Coin2 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, SpawnTransform);


		ObstaclesArray.Add(Coin2);
		//////////////////////////////////////////////////////////////////////////Third Coin//////////////////////////////////////////////////////////////////////////

		ACoinsItems* Coin3;
		currentLane = _NewCoinLAne(LastCoinLane);
		switch (currentLane)
		{
		case 0:
		{
			if (CoinsLeftFrontType != -1)
				Coin3 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsLeftFront->GetComponentTransform());
			break;
		case 1:
			if (CoinsCenterFrontType != -1)
				Coin3 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsCenterFront->GetComponentTransform());
			break;
		case 2:
			if (CoinsRightFrontType != -1)
				Coin3 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsRightFront->GetComponentTransform());
			break;
		default:
			if (CoinsCenterFrontType != -1)
				Coin3 = GetWorld()->SpawnActor<ACoinsItems>(CoinClass, AttachPointCoinsCenterFront->GetComponentTransform());
			break;
		}
		if (Coin3)
			ObstaclesArray.Add(Coin3);
		}
	}
}

FVector AFloorTile::GetNearestPointInLane(const int Lane, const FVector position)
{
	FVector2f lane1 = FVector2f(LeftLane->GetComponentLocation().X, LeftLane->GetComponentLocation().Y);
	FVector2f lane2 = FVector2f(CenterLane->GetComponentLocation().X, CenterLane->GetComponentLocation().Y);
	FVector2f lane3 = FVector2f(RightLane->GetComponentLocation().X, RightLane->GetComponentLocation().Y);
	FVector2f lineDirVect = lane2 - lane1;
	FVector2f tangetVect = FVector2f(-lineDirVect.Y, lineDirVect.X);
	FVector2f point = FVector2f(position.X, position.Y);

	FVector2f vectPosLane;
	float dot;
	float t;
	FVector2f nearestPoint;
	switch (Lane)
	{
	case 0:
		vectPosLane = point - lane1;
		dot = FVector2f::DotProduct(vectPosLane, tangetVect);
		t = dot / FVector2f::DotProduct(tangetVect, tangetVect);
		nearestPoint = lane1 + (tangetVect * t);
		return FVector(nearestPoint.X, nearestPoint.Y, position.Z);
		break;
	case 1:
		vectPosLane = point - lane2;
		dot = FVector2f::DotProduct(vectPosLane, tangetVect);
		t = dot / FVector2f::DotProduct(tangetVect, tangetVect);
		nearestPoint = lane2 + (tangetVect * t);
		return FVector(nearestPoint.X, nearestPoint.Y, position.Z);
		break;
	case 2:
		vectPosLane = point - lane3;
		dot = FVector2f::DotProduct(vectPosLane, tangetVect);
		t = dot / FVector2f::DotProduct(tangetVect, tangetVect);
		nearestPoint = lane3 + (tangetVect * t);
		return FVector(nearestPoint.X, nearestPoint.Y, position.Z);
		break;
	default:
		break;
	}

	
	return FVector(0, 0, 0);
}

void AFloorTile::SpawnLaneItem(UArrowComponent* Lane, int currentRelatedCoinPosition)
{
	//I wanted to use int pointer here but for some reason unreal engine doesn't like it so let's do with some ugly switchs
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	const int RandVal = FMath::RandRange(0, 3);
	//ObstacleType = RandVal;
	AActor* Obstacle;
	int a = 0;
	switch (RandVal)
	{
	case 0:
		Obstacle = GetWorld()->SpawnActor<AObstacles>(SmallObstacleClass, SpawnLocation, SpawnParameters);
		switch (currentRelatedCoinPosition)
		{
		case 0:
			CoinsLeftLaneType = 1;
			break;
		case 1:
			CoinsCenterLaneType = 1;
			break;
		case 2:
			CoinsRightLaneType = 1;
			break;
		default:
			break;
		}
		break;
	case 1:
		//security to avoid 3 big obstacles in a row
		
		if (CoinsLeftLaneType == 3)
			a++;
		if (CoinsCenterLaneType == 3)
			a++;
		if (CoinsRightLaneType == 3)
			a++;
		if (a == 3)
			return;
		if (CoinsLeftLaneType == -1 || CoinsRightLaneType == -1 )
			return;
		Obstacle = GetWorld()->SpawnActor<AObstacles>(BigObstacleClass, SpawnLocation, SpawnParameters);
		switch (currentRelatedCoinPosition)
		{
		case 0:
			CoinsLeftLaneType = 3;
			break;
		case 1:
			CoinsCenterLaneType = 3;
			break;
		case 2:
			CoinsRightLaneType = 3;
			break;
		default:
			break;
		}
		break;
	case 2:
		Obstacle = GetWorld()->SpawnActor<AObstacles>(SmallObstacleUpClass, SpawnLocation, SpawnParameters);
		switch (currentRelatedCoinPosition)
		{
		case 0:
			CoinsLeftLaneType = 2;
			break;
		case 1:
			CoinsCenterLaneType = 2;
			break;
		case 2:
			CoinsRightLaneType = 2;
			break;
		default:
			break;
		}
		break;
		default:
		Obstacle = GetWorld()->SpawnActor<AObstacles>(SmallObstacleClass, SpawnLocation, SpawnParameters);
		switch (currentRelatedCoinPosition)
		{
		case 0:
			CoinsLeftLaneType = 1;
			break;
		case 1:
			CoinsCenterLaneType = 1;
			break;
		case 2:
			CoinsRightLaneType = 1;
			break;
		default:
			break;
		}
		break;
	case 3:
		Obstacle = GetWorld()->SpawnActor<ABooster>(BoosterClass, SpawnLocation, SpawnParameters);
		break;

	}
	ObstaclesArray.Add(Obstacle);
}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter)
	{
		RunGameMode->LaneSwitchValues[0] = FVector2f(LeftLane->GetComponentLocation().X, LeftLane->GetComponentLocation().Y);
		RunGameMode->LaneSwitchValues[1] = FVector2f(CenterLane->GetComponentLocation().X, CenterLane->GetComponentLocation().Y);
		RunGameMode->LaneSwitchValues[2] = FVector2f(RightLane->GetComponentLocation().X, RightLane->GetComponentLocation().Y);
		RunGameMode->AddFloorTile(true);

		RunCharacter->SetCurrentFloor(this);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 5.f, false);
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}
	while (!ObstaclesArray.IsEmpty())
	{
		AActor* actor = ObstaclesArray.Pop();
		if (actor)
			actor->Destroy();
	}
	RunGameMode->FloorTilesArray.Remove(this);
	this->Destroy();
}

