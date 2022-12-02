// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Portal.h"
#include "PhysicCalculations.h"
#include "Project1Character.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
}

APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	///Initialize Components
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	//BorderMesh
	PortalBorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	PortalBorderMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PortalBorderMesh->SetupAttachment(RootComponent);
	//Body Mesh
	PortalBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	PortalBodyMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PortalBodyMesh->SetupAttachment(RootComponent);

	//Colliders
	PortalCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Portal Collision"));
	PortalCollision->SetBoxExtent(FVector(100, 100, 175));
	PortalCollision->SetupAttachment(RootComponent);

	// Transfom to adjust the values os the target render
	BackFacingSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BackFacingScene"));
	BackFacingSceneComponent->SetupAttachment(RootComponent);
	BackFacingSceneComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	//Scene Capture
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent->bOverride_CustomNearClippingPlane = true;

	bCanTeleport = true;
}


void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bOverlapping)
	{
		Teleport();
	}

	if (OtherPortal != nullptr)
	{
		ConfigureRenderTargets();
	}
}

void APortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AProject1Character* OtherActorCharacter = Cast<AProject1Character>(OtherActor);
	if (OtherActorCharacter != nullptr)
	{
		bOverlapping = true;
		if (PortalSurface != nullptr && OtherPortal != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Portal overlapped the character"));
			PortalSurface->SetActorEnableCollision(false);
			CharacterP = OtherActorCharacter;
		}
		// If character overlapped this portal, set it to ignore portal walls

		OtherActorCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("ignore"), true);
	}
}

void APortal::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor->IsA(AProject1Character::StaticClass()))
	{
		bOverlapping = false;
		if (PortalSurface != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn did not overlapped the character"));
			PortalSurface->SetActorEnableCollision(true);
		}
	}
}

void APortal::ResizeRenderTarget(UTextureRenderTarget2D* RenderTarget2D, float SizeX, float SizeY)
{
	if (RenderTarget2D == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Render Target used to resize texture is nullptr"));
		return;
	}
	RenderTarget2D->ResizeTarget(SizeX, SizeY);
}

void APortal::Teleport()
{
	if (OtherPortal != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(
			GetWorld()->GetFirstPlayerController());
		if (PlayerController != nullptr)
		{
			AProject1Character* Character = Cast<AProject1Character>(PlayerController->GetCharacter());
			if (Character != nullptr)
			{
				if (UPhysicCalculations::CheckIsCrossingPortal(Character->GetActorLocation(), GetActorLocation(),
				                                               GetActorForwardVector(), bLastInFront, LastPosition))
				{
					FVector SavedVelocity = Character->GetCharacterMovement()->Velocity;

					FHitResult HitResult;
					FVector Location =
						UPhysicCalculations::ConvertLocation(Character->GetActorLocation(), this, OtherPortal);
					FRotator Rotation = UPhysicCalculations::ConvertRotation(
						Character->GetActorRotation(), this, OtherPortal);
					Character->SetActorLocationAndRotation(Location, Rotation, false, &HitResult,
					                                       ETeleportType::TeleportPhysics);
					PlayerController->SetControlRotation(
						UPhysicCalculations::ConvertRotation(PlayerController->GetControlRotation(), this,
						                                     OtherPortal));

					FVector NewVelocity =
						FVector::DotProduct(SavedVelocity, GetActorForwardVector()) * OtherPortal->
						GetActorForwardVector() +
						FVector::DotProduct(SavedVelocity, GetActorRightVector()) * OtherPortal->GetActorRightVector() +
						FVector::DotProduct(SavedVelocity, -GetActorUpVector()) * OtherPortal->GetActorUpVector();

					Character->GetCharacterMovement()->Velocity = -NewVelocity;

					LastPosition = Location;
					bOverlapping = false;
				}
			}
		}
	}
}

void APortal::SetTarget(APortal* NewTarget)
{
	if (NewTarget != nullptr)
	{
		Target = NewTarget;
	}
}

void APortal::ConfigureRenderTargets()
{
	const FTransform PlayerCameraTransform = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->
	                                         GetTransformComponent()->GetComponentTransform();

	// Get the Transform of the object and them Stores Location and rotation relative to the back
	const FTransform BackSceneRelativeToCameraTransform = UKismetMathLibrary::MakeRelativeTransform(
		PlayerCameraTransform, GetBackFacingSceneComponent()->GetComponentTransform());

	// OtherPortal has to be at tbe same direaction between him and the character
	OtherPortal->GetSceneCaptureComponent()->SetRelativeLocationAndRotation(
		BackSceneRelativeToCameraTransform.GetLocation(), BackSceneRelativeToCameraTransform.GetRotation());

	const float DistanceFromPlayerToThisPortal = UKismetMathLibrary::Vector_Distance(
		PlayerCameraTransform.GetLocation(), GetActorLocation());

	//Change The render target distance between the character and this
	OtherPortal->GetSceneCaptureComponent()->CustomNearClippingPlane = DistanceFromPlayerToThisPortal + 1.f;

	//Adjust RenderTarget
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		ResizeRenderTarget(GetSceneCaptureComponent()->TextureTarget, ViewportSize.X, ViewportSize.Y);
	}
}

APortal* APortal::GetOtherPortal() const
{
	return OtherPortal;
}

// Sets default values


void APortal::SetIsColorBluePortal(bool Value)
{
	bIsColorBlue = Value;
	SetPortalProperties();
}


void APortal::SetOtherPortal(APortal* Portal)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to set portal"));
	if (Portal != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Portal is set"));
		OtherPortal = Portal;
		SetRenderTargetsProperties();
	}
}

void APortal::SetPortalSurface(AActor* Surface)
{
	PortalSurface = Surface;
}

//Gets the portal surface
AActor* APortal::GetSurfaceForPortal() const
{
	return PortalSurface;
}

//Return the RenderTarget
UTextureRenderTarget2D* APortal::GetRenderTarget()
{
	return RenderTarget;
}
