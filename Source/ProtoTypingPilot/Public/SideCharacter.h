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

	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void TurnToMouse();






private:
	bool bCheckMousePositonTurnOnce{ true };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	
};
