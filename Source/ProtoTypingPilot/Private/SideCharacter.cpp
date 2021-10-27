// Fill out your copyright notice in the Description page of Project Settings.


#include "SideCharacter.h"
#include "MainCharacter.h"
#include "Components/SplineComponent.h"

// Sets default values
ASideCharacter::ASideCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void ASideCharacter::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ASideCharacter::SelfDestroy, 0.5f, false);
}

void ASideCharacter::SelfDestroy()
{
	Destroy();
}

void ASideCharacter::TurnToMouse()
{
	if(bCheckMousePositonTurnOnce)
	{
		FHitResult Hit;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		FVector CurrentLocation = GetActorLocation();
		FVector mouseHitLocation = Hit.Location;

		FVector ReturnValue = mouseHitLocation - CurrentLocation;
		ReturnValue.Rotation();
		FRotator hitTotalDirection(0.f, ReturnValue.Rotation().Yaw, 0.f);

		float rotationRate = this->GetActorRotation().Yaw;

		SetActorRotation(hitTotalDirection);
		bCheckMousePositonTurnOnce = false;
	}
}

//void ASideCharacter::CreatePredictionSpline()
//{
//	AMainCharacter* MainCharacterRef = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
//	if(!MainCharacterRef)
//	{
//		return;
//	}
//	MainCharacterRef->bRightMouseButtonPressed;
//	if (MainCharacterRef->bRightMouseButtonPressed)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ValidCast"));
//	}
//}



// Called every frame
void ASideCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurnToMouse();

	//DEBUGONLY!!!
	//CreatePredictionSpline();
}

// Called to bind functionality to input
void ASideCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

