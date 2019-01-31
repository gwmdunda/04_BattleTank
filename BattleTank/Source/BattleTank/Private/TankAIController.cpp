// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "TankMovementComponent.h"

ATankAIController::ATankAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!IsDeath)
	{
		auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATank::StaticClass(), FoundActors);
		if (FoundActors.Num() != 1)
		{
			AActor* AttackThis = nullptr;
			int32 DistanceToAttack = -1;

			for (int32 index = 1; index < FoundActors.Num(); ++index)
			{
				float TempDistance = FVector::Dist(GetPawn()->GetActorLocation(), FoundActors[index]->GetActorLocation());
				if ((DistanceToAttack == -1 || TempDistance < DistanceToAttack) && TempDistance > 1 && Cast<ATank>(FoundActors[index])->GetHealthPercent() > 0)
				{
					AttackThis = FoundActors[index];
					DistanceToAttack = FVector::Dist(GetPawn()->GetActorLocation(), FoundActors[index]->GetActorLocation());
				}
			}
			if (AttackThis)
			{
				//UE_LOG(LogTemp, Warning, TEXT("I found : %s"), *AttackThis->GetName())
				EPathFollowingRequestResult::Type RequestResult = MoveToActor(AttackThis, AcceptanceRadius, false);
				if (RequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
				{
					if (Cast<ATank>(AttackThis)->GetHealthPercent() > Cast<ATank>(GetPawn())->GetHealthPercent())
					{
						GetPawn()->FindComponentByClass<UTankMovementComponent>()->IntendMoveBackward(1);
					}
				}
				AimingComponent->AimAt(AttackThis->GetActorLocation() + FVector(0, 0, FMath::FRandRange(-100, 200)));
				if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
				{
					AimingComponent->Fire();
				}
			}
		}
	}
	else if(GetWorld()->GetTimeSeconds()-DeathTime > 5)
	{
		GetPawn()->SetActorLocation(StartingPosition);
		Cast<ATank>(GetPawn())->Heal();
		IsDeath = false;
		GetPawn()->FindComponentByClass<UTankAimingComponent>()->ResetAmmo();
	}
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	StartingPosition = GetPawn()->GetActorLocation();
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
	//GetPawn()->DetachFromControllerPendingDestroy();
	IsDeath = true;
	DeathTime = GetWorld()->GetTimeSeconds();
}

