// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSChatSystemWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UFPSChatSystemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFPSChatSystemWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnCommitedSendMessage(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnClickedClose();

	UFUNCTION(BlueprintCallable)
		void UpdateMessage();

	void AssignAnimations();

	UFUNCTION(BlueprintCallable)
	void OnBeginConstruct();

	UFUNCTION(BlueprintCallable)
	class UWidgetAnimation* GetOpenOrClose() { return OpenOrCloseAnim; };

public:
	UPROPERTY(BlueprintReadWrite, Category = Widget)
	TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode;

	class UButton* CloseButton;

	TArray<class UWidget*> Message;

	class UVerticalBox* MessageBox;

	UPROPERTY(BlueprintReadWrite, Category = Widget)
	class UScrollBox* ScrollBox;

	class UEditableTextBox* SendMessageBox;

	UPROPERTY(BlueprintReadWrite, Category = Widget)
	class UWidgetAnimation* OpenOrCloseAnim;
};
