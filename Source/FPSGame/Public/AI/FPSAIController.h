// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFPSAIController();

public:
	FName TargetActor;

	FName PatrolPoint;

	FName PatrolDestination;
};
