// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSPowerupActor.generated.h"

class AFPSCharacter;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API AFPSPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Powerups)
		float Speedup;

	UPROPERTY(EditDefaultsOnly, Category = Powerups)
		float PowerupInterval;

	// 应用增强型道具效果总时间
	UPROPERTY(EditDefaultsOnly, Category = Powerups)
		int32 TotalNrOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	int32 TicksProcessed;

	UFUNCTION()
		void OnTickPowerup();

	UPROPERTY(Replicated)
		TArray<AFPSCharacter*> Players;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetVisibility(UStaticMeshComponent* StaticComp1);
	void MulticastSetVisibility_Implementation(UStaticMeshComponent* StaticComp1);


public:
	
	/*
	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
		void OnActivated(AFPSCharacter* Player);

	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
		void OnExpired(AFPSCharacter* Player);
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
		void OnPowerupTicked();
	

	void ActivatePowerup(AActor* OtherActor);

	void OnActivatedC(AFPSCharacter* Player);

	void OnExpiredC(AFPSCharacter* Player);
	
	
};
