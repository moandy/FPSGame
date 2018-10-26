// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class AFPSPowerupActor;

UCLASS()
class FPSGAME_API AFPSPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		UDecalComponent* DecalComp;  // Ìí¼ÓÌù»¨

	UPROPERTY(EditDefaultsOnly, Category = PickupActor)
		TSubclassOf<AFPSPowerupActor> PowerupClass;

	AFPSPowerupActor* PowerUpInstance;

	UPROPERTY(EditDefaultsOnly, Category = PickupActor)
		float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ReSpwan();

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
