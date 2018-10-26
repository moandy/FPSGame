// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8 {
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void ResetOrientation();


	void SetGuardState(EAIState newState);


	void MoveToNextPatrolPoint();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
		void OnHealthChanged(float heal);

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
		void OnStateChanged(EAIState NewState);

	UFUNCTION(BlueprintCallable)
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION(BlueprintCallable)
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
		void OnRep_GuardState();

// 	UFUNCTION()
// 		void OnHealthChangeded(class UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void OnHealthHurt(class UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, Category = Component)
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class UFPSHealthComponent* HealthComp;

public:
// 	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Health)
// 		float health;

	UPROPERTY(EditInstanceOnly, Category = AI)
		bool bPatrol;    //是否巡逻

	UPROPERTY(EditInstanceOnly, Category = AI, meta = (EditCondition = "bPatrol"))
	TArray<AActor*> PatrolPoint;

	AActor* CurrentPatrolPoint;

	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
		EAIState GuardState;

	FRotator OriginalRotation;

	FTimerHandle TimerHandle_ResetOrientation;
	
};
