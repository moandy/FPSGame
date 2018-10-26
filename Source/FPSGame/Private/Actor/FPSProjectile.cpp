// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSAIGuard.h"
#include "FPSExplosiveBarrel.h"
#include "FPSCharacter.h"
#include "FPSAIBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Damage = 20.f;

	SetReplicates(true);
	SetReplicateMovement(true);
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float ActualDamage = DetermineSurface(Hit);

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector ShotDirection = EyeRotation.Vector();

	UGameplayStatics::ApplyPointDamage(OtherActor, ActualDamage, ShotDirection, Hit, GetOwner()->GetInstigatorController(), this, DamageType);

// 	if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor)) {
// 		Player->OnHealthHurt(ActualDamage);
// 	}


// 	if (AFPSAIGuard* AIGuard = Cast<AFPSAIGuard>(OtherActor))
// 	{
// 		AIGuard->OnHealthHurt(ActualDamage);
// 	}

// 	if (AFPSAIBase* FPSAI = Cast<AFPSAIBase>(OtherActor)) {
// 		FPSAI->OnHealthHurt(ActualDamage);
// 	}

	AFPSExplosiveBarrel* ExplosiveBarrel = Cast<AFPSExplosiveBarrel>(OtherActor);
// 	if (ExplosiveBarrel)
// 	{
// 		ExplosiveBarrel->OnHealthHurt(ActualDamage);
// 	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && ExplosiveBarrel == nullptr)
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 33.0f, GetActorLocation());
		
	}
	else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && ExplosiveBarrel != nullptr) 
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 30.0f, GetActorLocation());
	}

	if (Role < ROLE_Authority) {
		check(HitUp);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitUp, GetActorLocation());  //生成爆炸效果
	}

	if (Role == ROLE_Authority) {
		MakeNoise(1.0f, Instigator);

		Destroy();
	}
	
}

float AFPSProjectile::DetermineSurface(FHitResult Hit)
{
	float ActualDamage;
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	switch (SurfaceType)
	{
	case SurfaceType_Default:
		HitUp = DefaultHitUp;
		ActualDamage = Damage;
		break;
	case SurfaceType1:
		HitUp = DefaultHitUp;
		ActualDamage = Damage;
		break;
	case SurfaceType2:
		HitUp = HitHeadUp;
		ActualDamage = Damage * 2;
		break;
	default:
		HitUp = DefaultHitUp;
		ActualDamage = Damage;
		break;
	}
	return ActualDamage;
}
