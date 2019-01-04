// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
ATankPlayerController::ATankPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (AimingComponent)
	{
		FoundAimingComponent(AimingComponent);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}



void ATankPlayerController::AimTowardsCrosshair()
{
	if(!GetPawn()){ return; }
	FVector HitLocation;
	//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"),HitLocation.ToString())
	if (GetSightRayHitLocation(HitLocation))
	{
		GetPawn()->FindComponentByClass<UTankAimingComponent>()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	FVector2D ScreenPosition = ScreenLocation();
	FVector WorldLocation;
	if (GetLookDirection(ScreenPosition, WorldLocation))
	{
		return GetLookVectorHitLocation(HitLocation,WorldLocation);
	}
	return false;
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

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossesedTank = Cast<ATank>(InPawn);
		if (!ensure(PossesedTank)) { return; }
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossedTankDeath);
	}
}

void ATankPlayerController::OnPossedTankDeath()
{
	StartSpectatingOnly();
}