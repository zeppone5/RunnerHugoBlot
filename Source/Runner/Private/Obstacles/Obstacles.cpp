#include "Obstacles/Obstacles.h"
#include "../../Public/Charaters/RunCharacter.h"

AObstacles::AObstacles()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComponent);

	StaticMesh->OnComponentHit.AddDynamic(this, &AObstacles::OnObstacleHit);
}

void AObstacles::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

	if (RunCharacter)
	{
		if (RunCharacter->bIsInv) {
			Destroy();
		}
		else
			RunCharacter->Death();
	}
}
