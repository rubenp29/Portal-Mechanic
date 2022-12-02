// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"


class UTextureRenderTarget2D;

UCLASS()
class PROJECT1_API APortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortal();

	virtual void Tick(float DeltaTime) override;

	/** Returns the scene capture component of the portal */
	FORCEINLINE USceneCaptureComponent2D* GetSceneCaptureComponent() const { return SceneCaptureComponent; }

	/** Returns the back facing scene of the portal */
	FORCEINLINE USceneComponent* GetBackFacingSceneComponent() const { return BackFacingSceneComponent; }

	//Function to differentiate whether the spawned portal is portal blue or orange 
	UFUNCTION(BlueprintCallable, Category = "DifferentiatePortals")
	void SetIsColorBluePortal(bool Value);

	//Bool to distinguish whether the spawn portal is blue or orange
	UFUNCTION(BlueprintImplementableEvent, Category="DistinguishPortals")
	bool ChangePortalColor(bool Value);

	UFUNCTION(BlueprintCallable, Category = "SetOtherPortals")
	void SetOtherPortal(APortal* Portal);

	void SetPortalSurface(AActor* Surface);


	AActor* GetSurfaceForPortal() const;


	UTextureRenderTarget2D* GetRenderTarget();
	APortal* GetOtherPortal() const;


private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Default scene root */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

	//Portal Border Configurations for the editor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PortalBorderMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PortalBodyMesh;
	//Portal Collider
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal", meta=(AllowPrivateAccess = "true"))
	UBoxComponent* PortalCollision;
	//Portal RenderTarget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Capture", meta=(AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* RenderTarget;
	//Portal SceneComponent use for the view part
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Capture", meta=(AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	USceneComponent* BackFacingSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DistinguishPortals", meta = (AllowPrivateAccess = "true"))
	bool bIsColorBlue;


protected:
	// Event called to update portal's properties whether it is blue or yellow 
	UFUNCTION(BlueprintImplementableEvent, Category = "DifferentiatePortals")
	void SetPortalProperties();

	/** Event called when this portal is linked to another. Material of this portal is the one it is connected to */
	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void SetRenderTargetsProperties();

	/** Begin Overlap Event */
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** End Overlap Event */
	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	// Portal I am linked to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal", meta = (BlueprintProtected = "true"))
	APortal* OtherPortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	APortal* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "bTeleport", meta = (BlueprintProtected = "true"))
	bool bCanTeleport;

	UFUNCTION(BlueprintCallable, Category="PortalRender", meta = (BlueprintProtected = "true"))
	void ResizeRenderTarget(UTextureRenderTarget2D* RenderTarget2D, float SizeX, float SizeY);

	UFUNCTION(BlueprintCallable, Category = "Teleport", meta = (BlueprintProtected = "true"))
	void Teleport();

	UFUNCTION(BlueprintCallable, Category = "Targe", meta = (BlueprintProtected = "true"))
	void SetTarget(APortal* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "SceneCapture", meta = (BlueprintProtected = "true"))
	void ConfigureRenderTargets();


private:
	bool bLastInFront;
	bool bOverlapping;
	FVector LastPosition;
	AActor* CharacterP;
	AActor* PortalSurface;
};
