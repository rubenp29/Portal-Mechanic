// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PortalManager.h"
#include "GameFramework/Actor.h"
#include "Project1Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AProject1Projectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AProject1Projectile();


	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	//projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Portal)
	TSubclassOf<APortal> PortalClass;

	/** Set bshouldSpawnPortal to value. Called by player depending on portal Blue or Orange */
	UFUNCTION(BlueprintCallable, Category = "PortalSpawn")
	void SetShouldSpawnPortal(bool Value);

	//Change projectile color
	UFUNCTION(BlueprintCallable, Category = "ProjectileColor")
	void SetBaseColor(bool Value);

protected:
	virtual void BeginPlay();

	// called depending on the button pressed for portals
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProjectileColor();

	// called depending on the button pressed for portals
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalSpawn", meta = (BlueprintProtected = "true"))
	bool bShouldSpawnPortal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalSpawn", meta = (BlueprintProtected = "true"))
	bool bChangeBaseMaterial;
};
