// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "Engine/World.h"
#include "FPSPlayerController.h"
#include "FPSAIGuard.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Perception/PawnSensingComponent.h"
#include "WidgetLayoutLibrary.h"



void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
		if (PC) {
			//游戏结束，显示鼠标出来。
			PC->bShowMouseCursor = true;
			if (PC->Role != ROLE_Authority)
				PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			

			//找到所有的AI，然后禁止它的行动。
			APawn* MyPawn = PC->GetPawn();
			if (MyPawn) {
				MyPawn->DisableInput(PC);
			}
		}
	}
}
