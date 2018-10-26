// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerController.h"
#include "FPSPlayerState.h"
#include "FPSChatSystemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetBlueprintLibrary.h"
#include "FPSHUD.h"
#include "GameUI/ObejectiveinfoWidget.h"
#include "GameUI/GameOverWidget.h"




AFPSPlayerController::AFPSPlayerController()
{
	//bShowMouseCursor = true;
}

void AFPSPlayerController::OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (Role < ROLE_Authority) {
		UClass* BPClass = LoadClass<UGameOverWidget>(NULL, TEXT("WidgetBlueprint'/Game/Blueprints/UI/GameUI/WBP_GameOver.WBP_GameOver_C'"));
		AFPSHUD* MyHud = Cast<AFPSHUD>(GetHUD());
		if (BPClass&&IsLocalController() && MyHud) {

			if (MyHud->MissionWidget) {
				MyHud->MissionWidget->RemoveFromViewport();
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, "RemoveMissionWidget");
			}

			MyHud->GameOverWidget = CreateWidget<UGameOverWidget>(this, BPClass);
			MyHud->GameOverWidget->bIsMissionComplete = bMissionSuccess;
			MyHud->GameOverWidget->AddToViewport();


		}
	}
}

void AFPSPlayerController::OpenChatWindow()
{
	AFPSHUD* MyHud = Cast<AFPSHUD>(GetHUD());

	if (MyHud->ChatWidget) {
		MyHud->ChatWidget->PlayAnimation(MyHud->ChatWidget->GetOpenOrClose(), 0.f, 1, MyHud->ChatWidget->PlayMode.GetValue());

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (MyHud->ChatWidget->PlayMode == EUMGSequencePlayMode::Forward) {
			MyHud->ChatWidget->PlayMode = EUMGSequencePlayMode::Reverse;
			if (PC)
			{
				UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
				PC->bShowMouseCursor = false;
			}
		}
		else
		{
			MyHud->ChatWidget->PlayMode = EUMGSequencePlayMode::Forward;
			if (PC)
			{
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, MyHud->ChatWidget);
				PC->bShowMouseCursor = true;
			}
		}
	}
}
