// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPowerupActor.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
AFPSPowerupActor::AFPSPowerupActor()
{
	Speedup = 1.3f;
	PowerupInterval = 10.f;
	TotalNrOfTicks = 1;
	TicksProcessed = 0;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSPowerupActor::BeginPlay()
{
	Super::BeginPlay();

	//SetReplicates(true);
}

void AFPSPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();

	if (TotalNrOfTicks <= TicksProcessed)
	{
		// 去除效果
		if (Players.Num() > 0) {
			OnExpiredC(Players[0]);
			Players.RemoveAt(0);
		}
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}


void AFPSPowerupActor::MulticastSetVisibility_Implementation(UStaticMeshComponent* StaticComp1)
{
	StaticComp1->SetVisibility(false);
}



void AFPSPowerupActor::ActivatePowerup(AActor* OtherActor)
{
	AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);
	// 施加效果
	if (Player == nullptr) return;
	Players.Push(Player);
	OnActivatedC(Player);

	if (PowerupInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &AFPSPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void AFPSPowerupActor::OnActivatedC(AFPSCharacter* Player)
{
	UCharacterMovementComponent* Comp = Cast<UCharacterMovementComponent>(Player->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	Comp->MaxWalkSpeed = Comp->GetMaxSpeed() * Speedup;
	UStaticMeshComponent* StaticComp1 = Cast<UStaticMeshComponent>(this->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	//StaticComp->SetVisibility(false);
	if (Role == ROLE_Authority) {
		MulticastSetVisibility(StaticComp1);
	}
}

void AFPSPowerupActor::OnExpiredC(AFPSCharacter* Player)
{
	UCharacterMovementComponent* Comp = Cast<UCharacterMovementComponent>(Player->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	Comp->MaxWalkSpeed = Comp->GetMaxSpeed() / Speedup;
	Destroy();
}

void AFPSPowerupActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSPowerupActor, Players);
}
