// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "FPSPowerupActor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"


// Sets default values
AFPSPickupActor::AFPSPickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.f);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));

	RootComponent = SphereComp;
	DecalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	DecalComp->DecalSize = FVector(64.f, 75.f, 75.f);
	DecalComp->SetupAttachment(RootComponent);

	CooldownDuration = 10.f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSPickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		ReSpwan();
	}
}

void AFPSPickupActor::ReSpwan()
{
	if (PowerupClass == nullptr) {
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerUpInstance = GetWorld()->SpawnActor<AFPSPowerupActor>(PowerupClass, GetTransform(), SpawnParams);


}

void AFPSPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PowerUpInstance) {
		PowerUpInstance->ActivatePowerup(OtherActor);
		PowerUpInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AFPSPickupActor::ReSpwan, CooldownDuration);
	}
}


