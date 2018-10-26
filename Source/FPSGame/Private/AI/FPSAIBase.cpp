// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"
#include "FPSHealthComponent.h"


// Sets default values
AFPSAIBase::AFPSAIBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	//HealthSum = Health = 200.f;

	HealthComp = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("HealthComp"));
	
}



void AFPSAIBase::BeginPlay()
{
	Super::BeginPlay();

	//HealthComp->OnHealthChanged.AddDynamic(this, &AFPSAIBase::OnHealthChangeded);
	HealthComp->OnHealthChanged.AddDynamic(this, &AFPSAIBase::OnHealthHurt);
}



void AFPSAIBase::OnHealthHurt(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (tHealth <= 0.0f) {
		Destroy();
	}
}

// void AFPSAIBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 
// 	//DOREPLIFETIME(AFPSAIBase, Health);
// }

