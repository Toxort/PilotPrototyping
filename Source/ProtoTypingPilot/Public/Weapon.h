// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UAudioComponent;

UCLASS()
class PROTOTYPINGPILOT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION(BlueprintCallable, Category = "Component")
		void Fire();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		USkeletalMeshComponent* MeshComp;


	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	float magazineSize;

	float bulletsLeft;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	UAudioComponent* ShootSound;

	UPROPERTY(BlueprintReadWrite, Category = "Sounds")
		bool bPlaySound;

	void playSound();

	void Reload();

	bool bIsReloading{ false };




public:	

};
