// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/WeaponBaseMeshComponent.h"
#include "WeaponRifeMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (COOP), meta = (BlueprintSpawnableComponent))
class FPSGAME_API UWeaponRifeMeshComponent : public UWeaponBaseMeshComponent
{
	GENERATED_BODY()
	
	
public:

	UWeaponRifeMeshComponent();

	UFUNCTION(Server, Reliable, WithValidation)
		virtual	void ServerFire();
	void ServerFire_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		virtual void MulticastPlayAnim();
	void MulticastPlayAnim_Implementation();

	/** Fires a projectile. */
	virtual void BeginFire();

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<class AFPSProjectile> ProjectileClass;
	
};
