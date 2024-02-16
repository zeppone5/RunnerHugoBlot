#include "Charaters/RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "../../Public/GameMode/GameModeRunner.h"
#include "../../Public/Actors/Floors/FloorTile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ARunCharacter::ARunCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetCollisionProfileName(TEXT("NoCollision"));
	Shield->SetupAttachment(GetMesh());
	

}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(RunnerMappingContext, 0);
		}
	}

	RunGameMode = Cast<AGameModeRunner>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	Shield->SetVisibility(false);
	
}

void ARunCharacter::MyMoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MyMoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MyMoveDownStarted()
{
	UPawnMovementComponent* MovementComponent = GetMovementComponent();
	if (MovementComponent->IsFalling())
	{
		MovementComponent->Velocity = MovementComponent->Velocity + FVector(0, 0, -1000);
	}
	if (!bIsSliding)
	{
		SetActorScale3D(FVector(1.0f, 1.0f, 0.5f));
		bIsSliding = true;
		SlideTimer = 0.0f;
		//Crouch();
		SlideAnimateStart();
	}
}


void ARunCharacter::MyMoveDownCompleted()
{
	
	if (bIsSliding)
	{

		SetActorScale3D(FVector(1.0f, 1.0f, 1.f));
		bIsSliding = false;
		SlideTimer = 0.0f;
		//UnCrouch();
		SlideAnimateEnd();
	}
}


void ARunCharacter::Death()
{
	if (!bIsDead)
	{
		const FVector Location = GetActorLocation();

		UWorld* World = GetWorld();

		if (World)
		{
			bIsDead = true;
			DisableInput(nullptr);


			if (DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
			}
			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
			}

			GetMesh()->SetVisibility(false);

			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);
		}
	}
}

void ARunCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}

void ARunCharacter::OnDeath()
{
	bIsDead = false;

	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}

void ARunCharacter::SlideAnimateStart()
{
}

void ARunCharacter::SlideAnimateEnd()
{
}


void ARunCharacter::ChangeLane()
{
	CurrentLane = NextLane;
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{

	float z = GetCapsuleComponent()->GetComponentLocation().Z;
		FVector Location = GetActorLocation();
		FVector clane = FVector(RunGameMode->LaneSwitchValues[CurrentLane].X, RunGameMode->LaneSwitchValues[CurrentLane].Y, Location.Z);
		FVector nlane = FVector(RunGameMode->LaneSwitchValues[NextLane].X, RunGameMode->LaneSwitchValues[NextLane].Y, Location.Z);

		dest = Location + (nlane - clane) / 4;
	
	Location.X = FMath::Lerp(Location.X, dest.X, Value);
	Location.Y = FMath::Lerp(Location.Y, dest.Y, Value);

	SetActorLocation(Location);
}
void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::SetCurrentFloor(AFloorTile* floor)
{
	CurrentFloor = floor;
StartingRotation = GetControlRotation();
}

void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveLeft, ETriggerEvent::Started, this, &ARunCharacter::MyMoveLeft);
		//EnhancedInputComponent->BindAction(MoveLeft, ETriggerEvent::Completed, this, &ARunCharacter::MyMoveLeftExit);
		EnhancedInputComponent->BindAction(MoveDown, ETriggerEvent::Started, this, &ARunCharacter::MyMoveDownStarted);
		EnhancedInputComponent->BindAction(MoveDown, ETriggerEvent::Completed, this, &ARunCharacter::MyMoveDownCompleted);
		EnhancedInputComponent->BindAction(MoveRight, ETriggerEvent::Started, this, &ARunCharacter::MyMoveRight);
		//EnhancedInputComponent->BindAction(MoveRight, ETriggerEvent::Completed, this, &ARunCharacter::MyMoveRightExit);
		EnhancedInputComponent->BindAction(MyJump, ETriggerEvent::Started, this, &ARunCharacter::Jump);
	}
}


void ARunCharacter::Boost()
{
	bIsInv = true;
	invTimer = 5.0f;
	Shield->SetVisibility(true);
	for (size_t i = 0; i < RunGameMode->FloorTilesArray.Num(); i++)
	{
		RunGameMode->FloorTilesArray[i]->ActivateBridge();
	}
}

void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentFloor) {
		FVector loc = GetActorLocation();
		FVector newLoc = CurrentFloor->GetNearestPointInLane(CurrentLane, loc);
		newLoc = FMath::Lerp(loc, newLoc, 0.1f);
		SetActorLocation(newLoc);

		FVector endLoc = CurrentFloor->TileEnd->GetComponentLocation();
		FVector startLoc = CurrentFloor->TileStart->GetComponentLocation();
		FVector vectDir = endLoc - startLoc; // Vecteur directeur de la droite

		FVector vectPlayerEnd = newLoc - startLoc; // Vecteur entre la nouvelle position du joueur et la position de départ

		float t = FVector::DotProduct(vectPlayerEnd, vectDir) / FVector::DotProduct(vectDir, vectDir); // Paramètre t

		FVector nearestPoint = startLoc + (vectDir * t); // Point le plus proche sur la droite

		float distanceNearEnd = FVector::Distance(nearestPoint, startLoc);
		float distanceStartEnd = FVector::Distance(startLoc, endLoc);
		float a = FMath::Clamp(distanceNearEnd / distanceStartEnd, 0.0f, 1.0f);

		DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Green, false, 0.1f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), nearestPoint, 10.0f, 12, FColor::Red, false, 0.1f);
		DrawDebugSphere(GetWorld(), endLoc, 10.0f, 12, FColor::Blue, false, 0.1f);
		DrawDebugSphere(GetWorld(), startLoc, 10.0f, 12, FColor::Green, false, 0.1f);

		FRotator rot = FMath::Lerp(StartingRotation, CurrentFloor->AttachPoint->GetComponentRotation(), a);
		GetController()->SetControlRotation(rot);

	}
	if (bIsSliding)
	{
		SlideTimer += DeltaTime;
		if (SlideTimer >= SlideTimeMax)
		{
			bIsSliding = false;
			SlideTimer = 0.0f;
			//UnCrouch();
			SlideAnimateEnd();

			SetActorScale3D(FVector(1.0f, 1.0f, 1.f));
		}
	}

	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.f;
	ControlRot.Pitch = 0.f;

	AddMovementInput(ControlRot.Vector());


	if (bIsInv) {
		invTimer -= DeltaTime;
		if (invTimer <= 0) {
			bIsInv = false;
			Shield->SetVisibility(false);
			for (size_t i = 0; i < RunGameMode->FloorTilesArray.Num(); i++)
			{
				RunGameMode->FloorTilesArray[i]->DesactivateBridge();
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.020f, FColor::Red, FString::Printf(TEXT("Coins : %i"), RunGameMode->TotalCoins));
}

