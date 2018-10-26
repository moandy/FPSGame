// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UTexture2D;

UCLASS()
class AFPSHUD : public AHUD
{
	GENERATED_BODY()

protected:

	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;

	

public:

	AFPSHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

public:
	void CreateMissionWidget(APlayerController* PC);

	void CreateChatWindow(APlayerController* PC);

	void RemoveWidgetFromHUD(UUserWidget* myWidget);

	class UFPSChatSystemWidget* ChatWidget;

	class UFPSMessageWidget* MessageWidget;

	class UGameOverWidget* GameOverWidget;

	class UObejectiveinfoWidget* MissionWidget;
};

