// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSMessageWidget.h"




UFPSMessageWidget::UFPSMessageWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PlayerName = "";
	Message = "";
}

void UFPSMessageWidget::SetMessage(FString PlayerName, FString Message)
{
	this->PlayerName = PlayerName;
	this->Message = Message;
	//this->TimeNow = FString::SanitizeFloat()
}
