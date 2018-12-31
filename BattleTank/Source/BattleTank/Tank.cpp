// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"




/*void ATank::Fire()
{
	if (!TankAimingComponent||(GetWorld()->GetTimeSeconds()- LastFireTime< ReloadTimeInSeconds)){ return; }
	LastFireTime = GetWorld()->GetTimeSeconds();
	TankAimingComponent->Fire(LaunchSpeed);

}*/

ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	LastFireTime = -ReloadTimeInSeconds;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


