// Fill out your copyright notice in the Description page of Project Settings.


#include "SideCharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void USideCharacterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void USideCharacterAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);

		MovementSpeed = LateralSpeed.Size();

		//bIsInAir = Pawn->GetMovementComponent()->IsFalling();


		direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		//UE_LOG(LogTemp, Warning, TEXT("MovementSpeed: %f, Direction: %f"), MovementSpeed, direction);

	}
}
