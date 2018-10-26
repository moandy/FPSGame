// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSHealthComponent.h"
#include "UnrealNetwork.h"


// Sets default values for this component's properties
UFPSHealthComponent::UFPSHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	DefaultHealth = 100.f;
	bIsDead = false;
	SetIsReplicated(true);
	HealthRate = 1.f;
	// ...
	Health = DefaultHealth;
}


// Called when the game starts
void UFPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ... 在服务器上响应伤害
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* MyOwner = GetOwner();
		if (MyOwner) {
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UFPSHealthComponent::HandleTakeAnyDamage);
		}
	}
	
	
}

void UFPSHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	HealthRate = Health / DefaultHealth;

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
	UE_LOG(LogTemp, Log, TEXT("HealthRate: %s"), *FString::SanitizeFloat(HealthRate));
	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UFPSHealthComponent::OnRep_Health()
{
	HealthRate = Health / DefaultHealth;
}

void UFPSHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFPSHealthComponent, Health);
}