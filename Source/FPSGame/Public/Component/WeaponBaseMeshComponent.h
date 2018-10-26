// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponBaseMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (COOP), meta = (BlueprintSpawnableComponent))
class FPSGAME_API UWeaponBaseMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UWeaponBaseMeshComponent();

		/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		USoundBase* FireFailSound;

public:

	virtual void BeginFire() {}

	virtual void StopFire() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlay)
		int32 BulletCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GamePlay)
		int32 BulletSum;
	
	
};
