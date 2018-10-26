// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NewFPSGameMode.generated.h"


class AActor;
/**
 * 
 */
UCLASS()
class FPSGAME_API ANewFPSGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Spectating)
		TSubclassOf<AActor> SpectatingViewpointClass;


public:

	ANewFPSGameMode();

	void CompleteMission(class APawn* InstigatorPawn, bool bMissionSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = GameMode)
		void OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccess);
	
	
};
