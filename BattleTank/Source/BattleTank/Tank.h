// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"
class UTankBarrel;
class UTurret;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:

	//UFUNCTION(BlueprintCallable)
	//void Fire();
private:
	// Sets default values for this pawn's properties
	ATank();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 10000;


	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float ReloadTimeInSeconds = 3;
	double LastFireTime;
};
