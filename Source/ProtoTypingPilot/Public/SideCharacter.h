// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SideCharacter.generated.h"

class USplineComponent;

UCLASS()
class PROTOTYPINGPILOT_API ASideCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASideCharacter();

	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//	UStaticMeshComponent* MeshComp;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	//	class USkeletalMesh* SkeletalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SelfDestroy();
	void TurnToMouse();

	void CreatePredictionSpline();
	void DestroyPredictionSpline();
	void DrawPredictionSpline();

	UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	USplineComponent* PredictionSpline;

	UPROPERTY(EditAnywhere, Category = "Throwing")
	float ThrowForce{ 1500 };

	UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	TSubclassOf<class AActor> BP_EndPoint;

private:
	bool bCheckMousePositonTurnOnce{ true };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	
};
