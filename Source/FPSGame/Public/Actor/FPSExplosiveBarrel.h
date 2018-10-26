// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExplosiveBarrel();
// 
// 	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Health, Category = RPC)
// 		float Health;
// 
// 	UFUNCTION()
// 		void OnRep_Health();
// 
// 	UFUNCTION()
// 		void OnHealthHurt(float Damage);

	UFUNCTION()
		void OnHealthHurt(class UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Components)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = Components)
		URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = Explosive)
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = Explosive)
		UMaterialInterface* ExplodedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class UFPSHealthComponent* HealthComp;

	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = Explosive)
		float ExplosionImpulse;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void PlayEffect();
};
