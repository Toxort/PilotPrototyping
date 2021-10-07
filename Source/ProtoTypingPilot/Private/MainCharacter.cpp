// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
//#include "TestTopDownShooter.h"
//#include "Bullet.h"
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

void AMainCharacter::Turn(float Value)
{
	FHitResult Hit;
	MyController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	FVector CurrentLocation = GetActorLocation();
	FVector hitLocation = Hit.Location;

	FVector ReturnValue = hitLocation - CurrentLocation;
	ReturnValue.Rotation();
	FRotator hitTotalDirection(0.f, ReturnValue.Rotation().Yaw, 0.f);

	SetActorRotation(hitTotalDirection);
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
}

