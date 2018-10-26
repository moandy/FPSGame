// Fill out your copyright notice in the Description page of Project Settings.

#include "NewFPSGameMode.h"

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

ANewFPSGameMode::ANewFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
}

void ANewFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn) {
		//InstigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass) {

			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			//AActor* NewViewTarget = nullptr;

			if (ReturnedActors.Num() > 0) {
				AActor* NewViewTarget = ReturnedActors[0];

				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PC = It->Get();
					if (PC) 
					{
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SpectaingViewpointClass is nullptr."))
		}


	}
	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS) {
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
	bUseSeamlessTravel = true;
}

