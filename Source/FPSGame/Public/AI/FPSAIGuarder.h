// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/FPSAIBase.h"
#include "FPSAIGuarder.generated.h"

UENUM(BlueprintType)
enum class EAIGuardState : uint8 {
	Idle,
	Suspicious,
	Alerted
};

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIGuarder : public AFPSAIBase
{
	GENERATED_BODY()
	
public:
	AFPSAIGuarder();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Component)
		class UPawnSensingComponent* PawnSensingComp;

 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
 		class UWidgetComponent* WidgetComp;

	UFUNCTION(BlueprintCallable)
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION(BlueprintCallable)
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	void ResetOrientation();


	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
		EAIGuardState GuardState;

	UFUNCTION()
		void OnRep_GuardState();

	void SetGuardState(EAIGuardState newState);

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
		void OnStateChanged(EAIGuardState NewState);


	
	UPROPERTY(EditInstanceOnly, Category = AI)
		bool bPatrol;    //是否巡逻

	bool bSeenPawn;

	UPROPERTY(EditInstanceOnly, Category = AI, meta = (EditCondition = "bPatrol"))
		TArray<AActor*> PatrolPoint;

	AActor* CurrentPatrolPoint;

	void MoveToNextPatrolPoint();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 	virtual void OnHealthHurt(float Damage) override;
// 
// 	virtual void OnRep_Health() override;

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
		void OnHealthChanged(float heal);

	UFUNCTION()
		virtual	void OnHealthHurt(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FTimerHandle TimerHandle_ResetOrientation;
};
