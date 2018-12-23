// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"


Atank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(getPawn());
}
