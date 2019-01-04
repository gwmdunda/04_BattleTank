// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ATankAIController::ATankAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATank::StaticClass(), FoundActors);
	if (FoundActors.Num() != 1)
	{
		AActor* AttackThis = FoundActors[0];
		int32 DistanceToAttack;
		if (FVector::Dist(GetPawn()->GetActorLocation(), AttackThis->GetActorLocation()) < 1 || Cast<ATank>(FoundActors[0])->GetHealthPercent() > 0.01) { DistanceToAttack = 100000000; }
		else { DistanceToAttack = FVector::Dist(GetPawn()->GetActorLocation(), AttackThis->GetActorLocation()); }
		for (int32 index = 1; index < FoundActors.Num(); ++index)
		{
			float TempDistance = FVector::Dist(GetPawn()->GetActorLocation(), FoundActors[index]->GetActorLocation());
			if ((TempDistance < DistanceToAttack) && TempDistance > 1 && Cast<ATank>(FoundActors[index])->GetHealthPercent()>0.01)
			{
				AttackThis = FoundActors[index];
				DistanceToAttack = FVector::Dist(GetPawn()->GetActorLocation(), FoundActors[index]->GetActorLocation());
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("I found : %s"), *AttackThis->GetName())
		MoveToActor(AttackThis, AcceptanceRadius,false);
		AimingComponent->AimAt(AttackThis->GetActorLocation()+FVector(0,0,FMath::FRandRange(-100,200)));
		if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
		{
			AimingComponent->Fire();
		}
	}
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossesedTank = Cast<ATank>(InPawn);
		if (!ensure(PossesedTank)) { return;  }
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
	}
}
void ATankAIController::OnPossedTankDeath()
{
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

