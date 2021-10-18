// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class AWeapon;

UCLASS()
class PROTOTYPINGPILOT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class USkeletalMesh* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class APlayerController* MyController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing")
	float distanceMouseToPlayer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	void Fire();

	void StartFire();

	void EndFire();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Dash();
	void DashExecute();
	bool DashOnce{ true };
	void ResetDash();
	void Turn(float Value);

	//void TurnCamera(float Value);
	//void Shoot();
	//void RightClickPressed();
	//void RightClickReleased();
	//void TestButton();
	//void TestAxis(float Value);


	void CaluculateDistanceBetweenMouseAndPlayer();

private:

	FVector mouseHitLocation;

	bool bRightClickPressed;

	float SpringArmYaw;

public:

	UPROPERTY(EditAnywhere, Category = "Movement")
		float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float TurnRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
