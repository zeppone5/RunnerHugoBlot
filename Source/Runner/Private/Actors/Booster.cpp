// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Booster.h"
#include "../../../../Source/Runner/Public/Charaters/RunCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "../../Public/Charaters/RunCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABooster::ABooster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	BoosterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoosterMesh"));
	BoosterMesh->SetupAttachment(SphereCollider);
	BoosterMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}



void ABooster::FuckOverlap(ARunCharacter* player)
{
	player->Boost();
}

void ABooster::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter)
	{

	}
}

// Called when the game starts or when spawned
void ABooster::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABooster::OnSphereOverlap);
}

void ABooster::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Booster Overlap"));
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter)
	{ 
		RunCharacter->Boost();
		Destroy();
	}
}

// Called every frame
void ABooster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

