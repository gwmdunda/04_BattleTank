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
	virtual void SetPawn(APawn* InPawn) override;
	UFUNCTION()
	void OnPossedTankDeath();
	float DeathTime;
	bool IsDeath = false;
	FVector StartingPosition;
public:
	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius = 10000;
};
