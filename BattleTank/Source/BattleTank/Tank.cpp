// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Projectile.h"





ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}


void ATank::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = StartingHealth;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	float DamageToApply= FMath::Clamp(DamagePoints, 0 , CurrentHealth);
	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0 && !IsDeath)
	{
			
			IsDeath = true;
			Deaths++;
			ATank* TankCauser = Cast<ATank>(DamageCauser);
			if (TankCauser) { TankCauser->Kills++; }
			OnDeath.Broadcast();
	}
	return DamageToApply;
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}

void ATank::Heal()
{
	CurrentHealth = StartingHealth;
	IsDeath = false;
}

int32 ATank::GetKills()
{
	return Kills;
}

int32 ATank::GetDeaths()
{
	return Deaths;
}
