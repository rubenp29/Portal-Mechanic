// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project1Projectile.h"
#include "PortalManager.h"
#include "Project1GameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AProject1Projectile::AProject1Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProject1Projectile::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AProject1Projectile::SetShouldSpawnPortal(bool Value)
{
	bShouldSpawnPortal = Value;
}

void AProject1Projectile::SetBaseColor(bool Value)
{
	bChangeBaseMaterial = Value;
	UE_LOG(LogTemp, Warning, TEXT("log set base color "));

	UpdateProjectileColor();
}

void AProject1Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProject1Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel2)
		{
			APortalPlayerController* PlayerController = Cast<APortalPlayerController>(
				GetWorld()->GetFirstPlayerController());
			APortalManager* PortalManager = PlayerController->GetPortalManager();
			PortalManager->SpawnPortal(Hit, bShouldSpawnPortal);
		}
	}
	Destroy();
}
