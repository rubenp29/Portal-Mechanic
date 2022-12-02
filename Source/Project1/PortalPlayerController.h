// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PortalPlayerController.generated.h"

class APortalManager;
UCLASS()
class PROJECT1_API APortalPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	APortalManager* GetPortalManager();
	UPROPERTY(EditDefaultsOnly, Category = Portal)
	TSubclassOf<class APortal> Portal;


protected:
	virtual void BeginPlay() override;
	APortalManager* PortalManager;
};
