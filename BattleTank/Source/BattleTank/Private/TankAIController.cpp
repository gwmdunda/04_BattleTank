// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

ATankAIController::ATankAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ATank* AITank = GetControlledTank();
	if (AITank)
	{
		ATank* PlayerTank = GetPlayerTank();
		if (PlayerTank)
		{
			AITank->AimAt(PlayerTank->GetActorLocation());
		}
	}
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("This is %s"), *PlayerTank->GetName())
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find any player"))
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const 
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}