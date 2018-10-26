// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAIBehaviorType : uint8
{
	/* Does not move, remains in place until a player is spotted */
	Passive,

	/* Patrols a region until a player is spotted */
	Patrolling,
};