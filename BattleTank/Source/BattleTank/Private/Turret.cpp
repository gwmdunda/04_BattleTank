// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

void UTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	float AngleChange = RelativeSpeed * MaxDegreesPerSecond*GetWorld()->DeltaTimeSeconds;
	SetRelativeRotation(FRotator(0, AngleChange + RelativeRotation.Yaw, 0));
}