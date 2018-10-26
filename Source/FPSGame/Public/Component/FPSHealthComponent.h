// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DelegateCombinations.h"
#include "FPSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UFPSHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class FPSGAME_API UFPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPSHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool bIsDead;

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditDefaultsOnly, BlueprintReadOnly, Category = HealthComponent)
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HealthComponent)
		float DefaultHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HealthComponent)
		float HealthRate;

	UFUNCTION()
		void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintAssignable, Category = Events)
		FOnHealthChangedSignature OnHealthChanged;


	FORCEINLINE float GetHealth() { return Health; }
	
	UFUNCTION()
		void OnRep_Health();
};
