// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerState.h"
#include "FPSMessageWidget.h"
#include "FPSCharacter.h"
#include "FPSHUD.h"
#include "FPSChatSystemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget.h"



void AFPSPlayerState::MulticastMessage_Implementation(const FString& Name, const FString& Message)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		UClass* BPClass = LoadClass<UFPSMessageWidget>(NULL, TEXT("WidgetBlueprint'/Game/Blueprints/UI/ChatUI/WBP_Message.WBP_Message_C'"));
		if (BPClass && (*It)->IsLocalController()) {
			AFPSHUD* MyHud = Cast<AFPSHUD>((*It)->GetHUD());
			if (MyHud) {
				MyHud->MessageWidget = CreateWidget<UFPSMessageWidget>((It)->Get(), BPClass);
				if (MyHud->MessageWidget) {
					MyHud->MessageWidget->SetMessage(Name, Message);
					if (MyHud->ChatWidget)
					{
						MyHud->ChatWidget->Message.Add(MyHud->MessageWidget);
					}
				}
			}
		}
	}
}

void AFPSPlayerState::ServerMessage_Implementation(const FString& Name, const FString& Message)
{
	MulticastMessage(Name, Message);
}

bool AFPSPlayerState::ServerMessage_Validate(const FString& Name, const FString& Message)
{
	return true;
}
