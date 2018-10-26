// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIBase.generated.h"

UCLASS()
class FPSGAME_API AFPSAIBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIBase();


public:	
	//void SetHealth(float OtherHealth) { HealthSum = Health = OtherHealth; };

	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UAIPerceptionComponent * AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class UFPSHealthComponent* HealthComp;
	
public:
// 	UPROPERTY(EditAnywhere, Category = Health)
// 	float HealthSum;
// 
// 	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Health, Category = Health)
// 	float Health;

	//UFUNCTION(BlueprintCallable)
	virtual	void OnHealthHurt(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
