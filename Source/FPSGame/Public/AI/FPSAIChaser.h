// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/FPSAIBase.h"
#include "FPSType.h"
#include "FPSAIChaser.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIChaser : public AFPSAIBase
{
	GENERATED_BODY()

public:
	AFPSAIChaser();

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY()
		APawn* TargetPawn;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		class UBehaviorTree* FPSBehaviorTree;

	virtual void BeginPlay() override;

public:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = AI)
	bool bDied;
	
	//virtual void OnHealthHurt(float Damage) override;

	UFUNCTION(BlueprintCallable)
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	virtual	void OnHealthHurt(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
