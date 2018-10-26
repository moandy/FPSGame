// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "FPSCharacter.h"
#include "FPSChatSystemWidget.h"
#include "UserWidget.h"
#include "WidgetBlueprintLibrary.h"
#include "FPSPlayerState.h"
#include "GameUI/ObejectiveinfoWidget.h"

AFPSHUD::AFPSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/UI/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X + 5.f),
										   (Center.Y + 20.0f + 5.f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetOwningPlayerController());

	CreateMissionWidget(PC);
	CreateChatWindow(PC);
}


void AFPSHUD::CreateMissionWidget(APlayerController* PC)
{
	//APlayerController* PC = Cast<APlayerController>(GetOwningPlayerController());
	UClass* BPClass = LoadClass<UObejectiveinfoWidget>(NULL,
		TEXT("WidgetBlueprint'/Game/Blueprints/UI/GameUI/WBP_Obejectiveinfo.WBP_Obejectiveinfo_C'"));
	if (PC&&BPClass)
	{
		MissionWidget = CreateWidget<UObejectiveinfoWidget>(PC, BPClass);
		MissionWidget->AddToViewport();
	}
}

void AFPSHUD::CreateChatWindow(APlayerController* PC)
{
	//APlayerController* PC = Cast<APlayerController>(GetOwningPlayerController());
	UClass* BPClass = LoadClass<UFPSChatSystemWidget>(NULL,
		TEXT("WidgetBlueprint'/Game/Blueprints/UI/ChatUI/WBP_ChatSystem.WBP_ChatSystem_C'"));
	if (PC&&BPClass)
	{
		ChatWidget = CreateWidget<UFPSChatSystemWidget>(PC, BPClass);
		ChatWidget->AddToViewport();
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
		PC->bShowMouseCursor = false;
	}
}

void AFPSHUD::RemoveWidgetFromHUD(UUserWidget* MyWidget)
{
	if (MyWidget) {
		MyWidget->RemoveFromViewport();
	}
}
