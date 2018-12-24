// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
ATankPlayerController::ATankPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Player = GetPawn();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if(!GetControlledTank()){ return; }
	FVector HitLocation;
	//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"),HitLocation.ToString())
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	FVector2D ScreenPosition = ScreenLocation();
	FVector WorldLocation;
	if (GetLookDirection(ScreenPosition, WorldLocation))
	{
		GetLookVectorHitLocation(HitLocation,WorldLocation);
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldDirection;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldDirection, LookDirection);
}

FVector2D ATankPlayerController::ScreenLocation() const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	return FVector2D(ViewportSizeX*CrosshairXLocation, ViewportSizeY*CrosshairYLocation);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector& HitLocation, FVector WorldLocation) const
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		(StartLocation + WorldLocation * LineTraceRange),
		ECollisionChannel::ECC_Visibility
	))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else
	{
		HitLocation = FVector(0);
		return false;
	}
}