// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Weapon.h"
#include "SideCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
//#include "TestTopDownShooter.h"
//#include "Bullet.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);


	//GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1250.0f;
	//->bInheritPitch = false;
	const FRotator springArmRotation = FRotator(60.f, 0.f, 0.f);
	SpringArmComp->bInheritYaw = false;
	//SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 7.f;
	SpringArmComp->bDoCollisionTest = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;


	MovementSpeed = 20;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyController = CreateDefaultSubobject<APlayerController>(TEXT("MyController"));
	TurnRate = 45.0f;

	SpringArmYaw = 0;

	bRightClickPressed = false;

	WeaponAttachSocketName = "RightHandSocket";

	SideCharacterSpawnPoint = { -100.f, 200.f, 300.f };

	SideCharacterThrowPoint = SideCharacterSpawnPoint + FVector{ 0,0,160 };

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	MyController = GetWorld()->GetFirstPlayerController();
	if (MyController)
	{
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Spawn Default Weapon
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	if(CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
}

void AMainCharacter::Fire()
{
	if(!bRightMouseButtonPressed)
	{
		CurrentWeapon->Fire();
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		//AddActorLocalOffset(Direction * Value * MovementSpeed * GetWorld()->GetDeltaSeconds());
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "Forward-Backward");
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		//AddActorLocalOffset(Direction * Value * MovementSpeed * GetWorld()->GetDeltaSeconds());
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "Right-Left");

		//SetActorRotation(FRotator(0, 0, 0));
	}
}

void AMainCharacter::Dash()
{
	FTimerHandle UnusedHandle;

	FVector velocity = GetVelocity().GetSafeNormal();
	if(DashOnce)
	{
		GetCharacterMovement()->AddImpulse(velocity * 5000, true);
		//LaunchCharacter(FVector(0, 0, 50), false,false
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AMainCharacter::ResetDash, 0.2, false);
		DashOnce = false;
	}
}

void AMainCharacter::ResetDash()
{
	DashOnce = true;
}

void AMainCharacter::TurnToMouse(float Value)
{
	FHitResult Hit;
	MyController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	FVector CurrentLocation = GetActorLocation();
	mouseHitLocation = Hit.Location;

	FVector ReturnValue = mouseHitLocation - CurrentLocation;
	ReturnValue.Rotation();
	FRotator hitTotalDirection(0.f, ReturnValue.Rotation().Yaw, 0.f);

	float rotationRate = this->GetActorRotation().Yaw;
	//UE_LOG(LogTemp, Warning, TEXT("TurnRateX: %f"), rotationRate);


	SetActorRotation(hitTotalDirection);
}

void AMainCharacter::Turn2Test(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Value: %f"), Value);
}

void AMainCharacter::SideCharacterThrow()
{
	if(!bSideCharacterOnCooldown)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<ASideCharacter>(BP_SideCharacter,
			GetActorLocation() + SideCharacterSpawnPoint,
			FRotator(0.f, 0.f, 0.f),
			SpawnParams);
		bSideCharacterOnCooldown = true;

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AMainCharacter::ResetSideCharacterCooldown, 2, false);
	}
}


void AMainCharacter::SetIsNotAimThrowing()
{
	bRightMouseButtonPressed = false;
	UE_LOG(LogTemp, Warning, TEXT("False"));
}

void AMainCharacter::SetLeftMouseTrue()
{
	Fire();
	bLeftClickedPressed = true;
}

void AMainCharacter::SetLeftMouseFalse()
{
	bLeftClickedPressed = false;
}

void AMainCharacter::SetIsAimThrowing()
{
	bRightMouseButtonPressed = true;
	UE_LOG(LogTemp, Warning, TEXT("True"));
}




void AMainCharacter::CaluculateDistanceBetweenMouseAndPlayer()
{
	distanceMouseToPlayer = (mouseHitLocation - GetActorLocation()).Size();
	//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distanceMouseToPlayer);
}

void AMainCharacter::ResetSideCharacterCooldown()
{
	bSideCharacterOnCooldown = false;
}

FRotator AMainCharacter::GetMouseDirection(FVector PlayerLocation)
{
	FHitResult Hit;
	MyController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	FVector CurrentLocation = {PlayerLocation.X,PlayerLocation.Y, 0};
	mouseHitLocation = Hit.Location;

	FVector ReturnValue = mouseHitLocation - CurrentLocation;
	ReturnValue.Rotation();
	FRotator hitTotalDirection(0.f, ReturnValue.Rotation().Yaw, 0.f);

	return hitTotalDirection;
}

FVector AMainCharacter::GetMousePosition()
{
	FHitResult Hit;
	MyController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	mouseHitLocation = Hit.Location;
	return mouseHitLocation;
}

//void AMainCharacter::CreatePredictionSpline()
//{
//	UWorld* const World = GetWorld();
//	if(World && IsValid(BP_EndPoint))
//	{
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.Instigator = this;
//	
//		World->SpawnActor<ASideCharacter>(BP_EndPoint,
//			FVector(100,100,300),
//			FRotator(0.f, 0.f, 0.f),
//			SpawnParams);
//		
//	}
//	
//}

//void AMainCharacter::DestroyPredictionSpline()
//{
//	if(IsValid(BP_EndPoint))
//	{
//		PredictionSpline->DestroyComponent();
//		BP_EndPoint;
//	}
//}

//void AMainCharacter::DrawPredictionSpline()
//{
//}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurnToMouse(0);
	CaluculateDistanceBetweenMouseAndPlayer();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::TurnToMouse);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	PlayerInputComponent->BindAxis("Turn2", this, &AMainCharacter::Turn2Test);

	// Fire and Throwing
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::SetLeftMouseTrue);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::SetLeftMouseFalse);
	PlayerInputComponent->BindAction("AimThrowing", IE_Pressed, this, &AMainCharacter::SetIsAimThrowing);
	PlayerInputComponent->BindAction("AimThrowing", IE_Released, this, &AMainCharacter::SetIsNotAimThrowing);
	PlayerInputComponent->BindAction("SideCharacter", IE_Pressed, this, &AMainCharacter::SideCharacterThrow);

}

