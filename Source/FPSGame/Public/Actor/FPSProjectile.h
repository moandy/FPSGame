// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class AFPSProjectile : public AActor
{
	GENERATED_BODY()

protected:

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* DefaultHitUp;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* HitHeadUp;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* HitUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
		TSubclassOf<UDamageType> DamageType;
public:

	AFPSProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damage;


public:
	float DetermineSurface(FHitResult Hit);

	void PlayEffect();
};

