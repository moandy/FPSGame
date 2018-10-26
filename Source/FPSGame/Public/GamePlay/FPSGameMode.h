// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = Spectating)
	TSubclassOf<AActor> SpectatingViewpointClass;

	UPROPERTY(EditAnywhere, Category = Spectating)
	AActor* ViewpointActor;

public:

	AFPSGameMode();

	
	void CompleteMission(class APawn* InstigatorPawn, bool bMissionSuccess);


// 	UFUNCTION(BlueprintImplementableEvent, Category = GameMode)
// 	void OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccess);


public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
// 	UFUNCTION()
// 	void OnQueryFinished(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);


};

