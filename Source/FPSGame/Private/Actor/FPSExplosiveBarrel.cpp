// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "FPSHealthComponent.h"


// Sets default values
AFPSExplosiveBarrel::AFPSExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;


	HealthComp = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("HealthComp"));
	HealthComp->Health = 20.f;

	ExplosionImpulse = 400.f;

	//Health = 60.f;

	

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AFPSExplosiveBarrel::OnHealthHurt(class UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		return;
	}
	if (tHealth <= 0.0f)
	{
		bExploded = true;

		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

		PlayEffect();

		RadialForceComp->FireImpulse();

	}
}

// Called when the game starts or when spawned
void AFPSExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay(); 
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AFPSExplosiveBarrel::OnHealthHurt);
}

void AFPSExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSExplosiveBarrel::PlayEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	MeshComp->SetMaterial(0, ExplodedMaterial);
}

// void AFPSExplosiveBarrel::OnRep_Health()
// {
// 	if (bExploded)
// 	{
// 		return;
// 	}
// 
// 	if (Health <= 0.0f)
// 	{
// 		bExploded = true;
// 
// 		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
// 		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
// 
// 		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
// 
// 		MeshComp->SetMaterial(0, ExplodedMaterial);
// 
// 		RadialForceComp->FireImpulse();
// 		
// 		
// 	}
// }
// 
// void AFPSExplosiveBarrel::OnHealthHurt(float Damage)
// {
// 	if (ROLE_Authority == Role) {
// 		Health -= Damage;
// 		if (Health <= 0.0f) Health = 0.0f;
// 	}	
// 	//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, GetFName().ToString() + FString::SanitizeFloat(Health));
// 	//OnRep_Health();
// }

// Called every frame


// void AFPSExplosiveBarrel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 
// 
// 	DOREPLIFETIME(AFPSExplosiveBarrel, Health);
// }
