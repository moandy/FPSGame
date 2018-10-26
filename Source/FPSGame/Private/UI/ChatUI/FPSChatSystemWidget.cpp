// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSChatSystemWidget.h"
#include "Text.h"
#include "SlateEnums.h"
#include "UnrealType.h"
#include "WidgetAnimation.h"
#include "UserWidget.h"
#include "Button.h"
#include "EditableTextBox.h"
#include "FPSCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerState.h"
#include "ScrollBox.h"
#include "FPSPlayerState.h"




UFPSChatSystemWidget::UFPSChatSystemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayMode = EUMGSequencePlayMode::Forward;
	CloseButton = nullptr;
	SendMessageBox = nullptr;
	OpenOrCloseAnim = nullptr;
	MessageBox = nullptr;
	ScrollBox = nullptr;
}

void UFPSChatSystemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UVerticalBox* box = Cast<UVerticalBox>(GetWidgetFromName("MessageBox")))
	{
		MessageBox = box;
		check(MessageBox);
	}

	if (UEditableTextBox* txtbox = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_0")))
	{
		SendMessageBox = txtbox;
		SendMessageBox->OnTextCommitted.AddDynamic(this, &UFPSChatSystemWidget::OnCommitedSendMessage);
		check(SendMessageBox);
	}

	if (UButton* btn = Cast<UButton>(GetWidgetFromName("CloseButton")))
	{
		CloseButton = btn;
		CloseButton->OnClicked.AddDynamic(this, &UFPSChatSystemWidget::OnClickedClose);
		check(CloseButton);
	}

	if (UScrollBox* scbox = Cast<UScrollBox>(GetWidgetFromName("ScrollBox1"))) {
		ScrollBox = scbox;
		check(ScrollBox);
	}

	AssignAnimations();
	check(OpenOrCloseAnim);
}

void UFPSChatSystemWidget::OnCommitedSendMessage(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter) {
		if (!Text.EqualTo(FText::FromString(""))) {
			//AFPSCharacter* player = Cast<AFPSCharacter>(GetOwningPlayerPawn());
			AFPSPlayerState* FPSPlayerState = GetOwningPlayerState<AFPSPlayerState>();
			if (FPSPlayerState) {
				FPSPlayerState->ServerMessage(FPSPlayerState->GetPlayerName(), Text.ToString());

				SendMessageBox->SetText(FText::FromString(""));
			}
		}
	}
}



void UFPSChatSystemWidget::OnClickedClose()
{
	PlayAnimation(OpenOrCloseAnim, 0.f, 1, PlayMode);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayMode == EUMGSequencePlayMode::Forward) {
		PlayMode = EUMGSequencePlayMode::Reverse;
		if (PC)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
			PC->bShowMouseCursor = false;
		}
	}
	else {
		PlayMode = EUMGSequencePlayMode::Forward;
// 		if (PC) {
// 			//UWidgetBlueprintLibrary::SetInputMode_UIOnly(PC, this)
// 		}
	}
	
	
}

void UFPSChatSystemWidget::UpdateMessage()
{
// 	MessageBox->ClearChildren();
// 	for (auto it : Message) {
// 		MessageBox->AddChild(it);
// 	}
	ScrollBox->ClearChildren();
	for (auto it : Message) {
		ScrollBox->AddChild(it);
	}
}

void UFPSChatSystemWidget::AssignAnimations()
{
	UProperty* prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (prop != nullptr)
	{
		// Only interested in object properties
		if (prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

			// Only want the properties that are widget animations
			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);

				if (widgetAnim != nullptr)
				{
					OpenOrCloseAnim = widgetAnim;
				}
			}
		}

		prop = prop->PropertyLinkNext;
	}
}

void UFPSChatSystemWidget::OnBeginConstruct()
{
	PlayAnimation(OpenOrCloseAnim, 0.f, 1, PlayMode);
	PlayMode = EUMGSequencePlayMode::Reverse;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC) {
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
		PC->bShowMouseCursor = false;
	}
}
