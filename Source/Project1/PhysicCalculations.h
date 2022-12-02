// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PhysicCalculations.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UPhysicCalculations : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PhysicCalculations", meta = (BlueprintProtected = "true"))
	static FVector ConvertLocation(FVector const& Location, AActor* Portal, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "PhysicCalculations", meta = (BlueprintProtected = "true"))
	static FRotator ConvertRotation(FRotator const& Rotation, AActor* Portal, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "PhysicCalculations", meta = (BlueprintProtected = "true"))
	static bool CheckIsInFrontOfPortal(FVector const& Point, FVector const& PortalLocation, FVector const& PortalNormal);

	UFUNCTION(BlueprintCallable, Category = "PhysicCalculations", meta = (BlueprintProtected = "true"))

	static bool CheckIsCrossingPortal(FVector const& Point, FVector const& PortalLocation, FVector const& PortalNormal,
	                            bool& out_LastInFront, FVector& out_LastPosition);
};
