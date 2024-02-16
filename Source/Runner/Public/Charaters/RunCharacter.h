// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "RunCharacter.generated.h"

UCLASS()
class RUNNER_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARunCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets)
	class UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets)
	class UStaticMeshComponent* Shield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Assets)
	class USoundBase* DeathSound;

	UFUNCTION() void Boost();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FVector dest;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneFinished();

	UFUNCTION()
	void SetCurrentFloor(AFloorTile* floor);

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION()
	void AddCoin();

	UPROPERTY(VisibleAnywhere) bool bIsInv = false;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* RunnerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MyJump;

	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	UPROPERTY()
	bool bIsDead = false;
	UPROPERTY()
	bool bIsSliding = false;

	UFUNCTION()	void MyMoveRight();

	UFUNCTION()	void MyMoveLeft();

	UFUNCTION()	void MyMoveDownStarted();

	UFUNCTION()	void MyMoveDownCompleted();

	UPROPERTY() float SlideTimer = 0.0f;
	UPROPERTY(EditAnywhere) float SlideTimeMax= 1.5f;

	UFUNCTION()	void OnDeath();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	class AGameModeRunner* RunGameMode;

	UPROPERTY(VisibleInstanceOnly) AFloorTile* CurrentFloor;

float invTimer = 0.0f;

	FRotator StartingRotation;

	UFUNCTION() void SlideAnimateStart();
	UFUNCTION() void SlideAnimateEnd();

};
