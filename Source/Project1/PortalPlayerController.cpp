// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalPlayerController.h"
#include "PortalManager.h"

APortalManager* APortalPlayerController::GetPortalManager()
{
	return PortalManager;
}

void APortalPlayerController::BeginPlay()
{
	PortalManager = GetWorld()->SpawnActor<APortalManager>(APortalManager::StaticClass(), FVector::ZeroVector,
	                                                       FRotator::ZeroRotator);
	PortalManager->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PortalManager->SetControllerOwner(this);
	PortalManager->SetPortalClass(Portal);
	Super::BeginPlay();
}
