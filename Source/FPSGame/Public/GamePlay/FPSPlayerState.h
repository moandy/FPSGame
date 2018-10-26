// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
//	TArray<class UUserWidget*> WidgetArray;

// 	UPROPERTY(BlueprintReadWrite)
// 	class UFPSChatSystemWidget* ChatWidget;

public:
// 	UFUNCTION(BlueprintCallable, Category = PlayerState)
// 		void AddToWidgetArray(UUserWidget* Widget);

	UFUNCTION(NetMulticast, Reliable, Category = RPC)
		void MulticastMessage(const FString& Name, const FString& Message);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMessage(const FString& Name, const FString& Message);
};
