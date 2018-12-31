// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

	ATankAIController();
	virtual void Tick(float DeltaTime) override;
	void BeginPlay() override;	
	float AcceptanceRadius = 4000;
};
