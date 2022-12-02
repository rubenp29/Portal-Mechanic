// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalManager.h"
#include "Portal.h"
#include "Project1Character.h"
#include "PortalPlayerController.h"
#include "Project1Projectile.h"


// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
}

void APortalManager::DestroyOldPortal(const FHitResult& HitComponent, APortal* NewPortal, bool bIsColorBlue)
{
	if (bIsColorBlue)
	{
		if (BluePortal != nullptr)
		{
			BluePortal->Destroy();
		}
		BluePortal = NewPortal;

		BluePortal->SetPortalSurface(HitComponent.GetActor());
	}
	else
	{
		if (OrangePortal != nullptr)
		{
			OrangePortal->Destroy();
		}

		OrangePortal = NewPortal;
		OrangePortal->SetPortalSurface(HitComponent.GetActor());
	}

	if (OrangePortal != nullptr && BluePortal != nullptr)
	{
		OrangePortal->SetOtherPortal(BluePortal);
		BluePortal->SetOtherPortal(OrangePortal);
	}
}


// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();
}

// Sets default values
APortalManager::APortalManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


void APortalManager::UpdatePortals()
{
	if (OrangePortal != nullptr && BluePortal != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePortals"));

		OrangePortal->SetOtherPortal(BluePortal);
		BluePortal->SetOtherPortal(OrangePortal);
	}
}

void APortalManager::SetControllerOwner(APortalPlayerController* NewOwner)
{
	OwningControllerPortal = NewOwner;
}

void APortalManager::SetPortalClass(TSubclassOf<APortal> Portal)
{
	PortalClass = Portal;
}

APortal* APortalManager::SpawnBluePortal(const FHitResult& HitComponent)
{
	APortal* NewPortal = nullptr;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Origin = HitComponent.Location + HitComponent.ImpactNormal;
	FRotator Rotation = HitComponent.ImpactNormal.Rotation();

	//Always Spawns the portal in the correct way
	NewPortal = GetWorld()->SpawnActor<APortal>(PortalClass, Origin, Rotation, ActorSpawnParameters);

	NewPortal->SetIsColorBluePortal(true);
	DestroyOldPortal(HitComponent, NewPortal, true);

	BluePortal = NewPortal;

	return BluePortal;
}


APortal* APortalManager::SpawnOrangePortal(const FHitResult& HitComponent)
{
	APortal* NewPortal = nullptr;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Origin = HitComponent.Location + HitComponent.ImpactNormal;
	FRotator Rotation = HitComponent.ImpactNormal.Rotation();

	//Always Spawns the portal in the correct way
	NewPortal = GetWorld()->SpawnActor<APortal>(PortalClass, Origin, Rotation, ActorSpawnParameters);

	NewPortal->SetIsColorBluePortal(false);
	DestroyOldPortal(HitComponent, NewPortal, false);
	OrangePortal = NewPortal;

	return OrangePortal;
}


void APortalManager::SpawnPortal(const FHitResult& HitComponent, bool Value)
{
	if (Value == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnBlue"));
		SpawnBluePortal(HitComponent);
	}

	else if (Value == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpanOrrange"));
		SpawnOrangePortal(HitComponent);
	}
}
