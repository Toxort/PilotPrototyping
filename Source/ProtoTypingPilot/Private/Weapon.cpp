// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include  "DrawDebugHelpers.h"
#include  "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "particles/ParticleSystemComponent.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDeugWeaponDrawing(
	TEXT("TOPDOWN.DebugWeapon"),
	DebugWeaponDrawing, 
	TEXT("Draw debugLines for Weapons"), 
	ECVF_Cheat);

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";

	magazineSize = 7;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	bulletsLeft = magazineSize;
}

void AWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner && bIsReloading == false)
	{
		//FVector EyeLocation;
		//FRotator EyeRotation;
		//MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//MyOwner->GetRootComponent()->


		// MEGA HACK i think the actorlocation and stuff i based on the wa



		FVector ShotDirection = MyOwner->GetActorRotation().Vector();


		FVector TraceStart = MyOwner->GetActorLocation() + FVector(0, 0, 55);
		FVector TraceEnd = TraceStart + (ShotDirection * 10000);

		//Particle "target" parameter
		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;



		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
		{
			// Blocking hit!  process damage

			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			if(ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
		}
		if(DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, .5f, 0.f, 3.0f);
		}

		if(MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}



		if(TracerEffect)
		{
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if(TracerComp)
			{
				TracerComp->SetVectorParameter("BeamEnd", TracerEndPoint);
			}

		}







		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AWeapon::Reload, 2, false);



		bIsReloading = true;
		UE_LOG(LogTemp, Warning, TEXT("Reload Start"));
	}
}

void AWeapon::Reload()
{
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload done"));
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

