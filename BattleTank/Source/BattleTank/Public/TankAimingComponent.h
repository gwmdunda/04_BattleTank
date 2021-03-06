// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"
UENUM()
enum class EFiringStatus : uint8
{
	Locked,
	Aiming,
	Reloading,
	Empty
};
//Forward Declaration
class UTankBarrel; 
class UTurret;
class AProjectile;
//Holds barrel's properties and elevate methods
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Aiming;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	int DefaultAmmo = 40;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	int AmmoRemaining = DefaultAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;
public:
	// Sets default values for this component's properties
	void ResetAmmo();
	void AimAt(FVector HitLocation);
	UTankAimingComponent();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTurret* TurretToSet);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Fire();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void FireGun();
	EFiringStatus GetFiringState();
private:
	UTankBarrel* Barrel = nullptr;
	void MoveBarrelTowards(FVector AimDirection);
	UTurret* Turret = nullptr;
	float LaunchSpeed = 90000;
	float ReloadTimeInSeconds_Cannon = 4;
	float ReloadTimeInSeconds_Gun = 0.05;
	double LastFireTime;
	double LastGunTime;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void BeginPlay() override;
	bool IsBarrelMoving();
	FVector AimDirection;
	float DamageIncreaseFactor = 1;
};
