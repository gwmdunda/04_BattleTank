 // Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Turret.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}



// Called when the game starts
void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTurret* TurretToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}



void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!Barrel || !Turret) { return;  }
	FVector TossVelocity(0);
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		TossVelocity,
		Barrel->GetSocketLocation(FName("Projectile")),
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if(bHaveAimSolution)
	{
		AimDirection = TossVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		
	}
	
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!Barrel || !Turret) { return; }
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimRotator - BarrelRotator;
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}

}

void UTankAimingComponent::Fire()
{
	if (FiringStatus == EFiringStatus::Reloading || AmmoRemaining == 0) { return; }
	LastFireTime = GetWorld()->GetTimeSeconds();
	if (!Barrel && !ProjectileBlueprint) { return; }
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
	if (Projectile)
	{
		Projectile->LaunchProjectile(LaunchSpeed,DamageIncreaseFactor);
		--AmmoRemaining;
		DamageIncreaseFactor = 1;
	}
}

EFiringStatus UTankAimingComponent::GetFiringState()
{
	return FiringStatus;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (AmmoRemaining == 0)
	{
		FiringStatus = EFiringStatus::Empty;
	}
	else if (GetWorld()->GetTimeSeconds() - LastFireTime < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else
	{
		if (DamageIncreaseFactor < 2.5)
		{
			DamageIncreaseFactor = 1+1.5*(GetWorld()->GetTimeSeconds() - LastFireTime)/8;
		}
		if (IsBarrelMoving())
		{
			FiringStatus = EFiringStatus::Aiming;
		}
		else
		{
			FiringStatus = EFiringStatus::Locked;
		}
	}
}

void UTankAimingComponent::BeginPlay()
{
	LastFireTime = 0;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	return !AimDirection.Equals(Barrel->GetForwardVector(), 0.01);
	
}
