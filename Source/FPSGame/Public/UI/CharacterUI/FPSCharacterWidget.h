// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UFPSCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = Character)
	class AFPSCharacter* OwnerCharater;
	
	

	virtual void NativeConstruct() override;
};
