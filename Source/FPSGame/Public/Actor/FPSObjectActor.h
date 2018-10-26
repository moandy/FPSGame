// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectActor.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSObjectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectActor();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent * MeshComp;

	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* PickupFX;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
