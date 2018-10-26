// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.h"
#include "UserWidget.h"
#include "WidgetLayoutLibrary.h"
#include "FPSPlayerState.h"
#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSAIController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));

	DefaultPawnClass = PlayerPawnClassFinder.Class;
	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();

}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{

	//Control the ViewTarget
	if (InstigatorPawn) {
		//InstigatorPawn->DisableInput(nullptr);
		if (ViewpointActor) {
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController* PC = It->Get();
				if (PC) {
					PC->SetViewTargetWithBlend(ViewpointActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else {
			if (SpectatingViewpointClass) {
				TArray<AActor*> ReturnedActors;
				UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
				//AActor* NewViewTarget = nullptr;
				if (ReturnedActors.Num() > 0) {
					ViewpointActor = ReturnedActors[0];
					for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
					{
						APlayerController* PC = It->Get();
						if (PC) {
							PC->SetViewTargetWithBlend(ViewpointActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
						}
					}
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("SpectaingViewpointClass is nullptr."))
			}
		}
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS) {
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It) {
		if (It->Get()) {
			AController* Controller = It->Get()->GetController();
			if (Cast<AFPSAIController>(Controller)) {
				Controller->StopMovement();
				Controller->UnPossess();
			}
		}
	}
	
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();


}

void AFPSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFPSGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//UE_LOG(LogTemp, Warning, TEXT("SpectaingViewpointClass is nullptr."))
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AFPSGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//UE_LOG(LogTemp, Warning, TEXT("SpectaingViewpointClass is nullptr."))
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

FString AFPSGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal /*= TEXT("")*/)
{
	//UE_LOG(LogTemp, Warning, TEXT("SpectaingViewpointClass is nullptr."))
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	//UE_LOG(LogTemp, Warning, TEXT("SpectaingViewpointClass is nullptr."))
	Super::PostLogin(NewPlayer);

}
