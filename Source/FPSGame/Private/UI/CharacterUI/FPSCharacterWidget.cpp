// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacterWidget.h"
#include "FPSCharacter.h"
#include "UserWidget.h"




void UFPSCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwningPlayerPawn())) {
		OwnerCharater = Player;
	}
}
