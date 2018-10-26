// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UFPSMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPSMessageWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetMessage(FString PlayerName, FString Message);

	UFUNCTION(BlueprintCallable)
		FString GetMessage() { return Message; };

	UFUNCTION(BlueprintCallable)
		FString GetPlayerName() { return PlayerName; };
public:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = Message)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = Message)
	FString Message;
	
	//FString TimeNow;
	
};
