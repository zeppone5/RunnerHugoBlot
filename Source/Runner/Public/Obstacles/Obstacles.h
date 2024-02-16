#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles.generated.h"

UCLASS()
class RUNNER_API AObstacles : public AActor
{
	GENERATED_BODY()

public:
	AObstacles();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
