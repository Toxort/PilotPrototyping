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




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "SideCharacter")
	TSubclassOf<class ASideCharacter> BP_SideCharacter;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	// Throwing
	//UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	//	class USplineComponent* PredictionSpline;

	//UPROPERTY(EditAnywhere, Category = "Throwing")
	//	float ThrowForce{ 1500 };

	//UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	//	TSubclassOf<class AActor> BP_EndPoint;

	void Fire();

	void StartFire();

	void EndFire();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Dash();
	bool DashOnce{ true };
	void ResetDash();
	void TurnToMouse(float Value);
	void Turn2Test(float Value);
	void SetIsAimThrowing();
	void SetIsNotAimThrowing();
	void SetLeftMouseTrue();
	void SetLeftMouseFalse();


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

	//SideCharacter Throw
	void ResetSideCharacterCooldown();
	void CreatePredictionSpline();
	void DestroyPredictionSpline();
	void DrawPredictionSpline();
public:

	UFUNCTION(BlueprintCallable, Category = "Throwing")
	FRotator GetMouseDirection(FVector PlayerLocation);

	UFUNCTION(BlueprintCallable, Category = "Throwing")
		FVector GetMousePosition();

	UPROPERTY(EditAnywhere, Category = "Movement")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float TurnRate;

	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void SideCharacterThrow();

	UPROPERTY(EditAnywhere, Category = "Throwing")
	float ThrowCooldownTime{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing")
		float distanceMouseToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwing")
		FVector SideCharacterSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwing")
		FVector SideCharacterThrowPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwing")
	bool bRightMouseButtonPressed{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwing")
	bool bLeftClickedPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwing")
	bool bSideCharacterOnCooldown{ false };
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
