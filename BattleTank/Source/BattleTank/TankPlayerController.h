// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"
class UTankAimingComponent;
/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const;
	FVector2D ScreenLocation() const;
	bool GetLookVectorHitLocation(FVector&, FVector) const;
	void SetPawn(APawn * InPawn);
	UFUNCTION()
	void OnPossedTankDeath();
public:
	ATankPlayerController();
	
	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AimTowardsCrosshair();
private:

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);
};
