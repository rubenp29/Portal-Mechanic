// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalPlayerController.h"
#include "Portal.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"


UCLASS()
class PROJECT1_API APortalManager : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PortalUpdate", meta = (BlueprintProtected = "true"))
	void UpdatePortals();
	// Sets default values for this actor's properties
	APortalManager();

	void SetControllerOwner(APortalPlayerController* NewOwner);

	void SetPortalClass(TSubclassOf<APortal> Portal);

	//Spawn Rad and Blue Portals
	APortal* SpawnOrangePortal(const FHitResult& HitComponent);
	APortal* SpawnBluePortal(const FHitResult& HitComponent);
	//Calls Portal

	void SpawnPortal(const FHitResult& HitComponent, bool Value);

	// Checks with Portal was spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DistinguishPortals", meta=(AllowPrivateAccess = "true"))
	bool bIsOrangePortal;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Function to swap the old portal for the new one */
	UFUNCTION(BlueprintCallable, Category = "DestroyPortal")
	void DestroyOldPortal(const FHitResult& HitComponent, APortal* NewPortal, bool bIsColorBlue);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<APortal> PortalClass;

	APortal* BluePortal;
	APortal* OrangePortal;
	UPROPERTY()
	APortalPlayerController* OwningControllerPortal;
};
