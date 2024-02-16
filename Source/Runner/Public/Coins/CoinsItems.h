#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinsItems.generated.h"

UCLASS()
class RUNNER_API ACoinsItems : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoinsItems();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UStaticMeshComponent* CoinMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	class USoundBase* OverlapSound;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
