// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;
	FTankDelegate OnDeath;
	void Heal();
protected:
	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetKills();
	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetDeaths();
	UPROPERTY(BlueprintReadOnly)
	bool IsDeath = false;

private:
	// Sets default values for this pawn's properties
	ATank();

	void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController * EventInstigator,AActor* DamageCauser) override;
	// Called to bind functionality to input
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	int32 StartingHealth = 200;
	int32 CurrentHealth;
	int32 Kills = 0;
	int32 Deaths = 0;
	
};
