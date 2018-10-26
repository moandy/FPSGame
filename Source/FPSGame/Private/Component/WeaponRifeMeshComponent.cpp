// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponRifeMeshComponent.h"
#include "FPSProjectile.h"
#include "ConstructorHelpers.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"




UWeaponRifeMeshComponent::UWeaponRifeMeshComponent()
{
	SetIsReplicated(true);

	static ConstructorHelpers::FClassFinder<AFPSProjectile> StaticProjectile(TEXT("Blueprint'/Game/Blueprints/BP_Actor/BP_Projectile.BP_Projectile_C'"));
	ProjectileClass = StaticProjectile.Class;
}

void UWeaponRifeMeshComponent::ServerFire_Implementation()
{
	if (ProjectileClass)
	{
		FVector MuzzleLocation = GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = GetSocketRotation("Muzzle");

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Instigator = Cast<AFPSCharacter>(GetOwner());
		ActorSpawnParams.Owner = GetOwner();

		//GetWorld()->LineTraceSingleByProfile()
		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);

		if (APlayerController* PC = Cast<APlayerController>(Cast<AFPSCharacter>(GetOwner())->GetController())) {
			if (Cast<AFPSCharacter>(Cast<AFPSCharacter>(GetOwner()))->FireCamShake) {
				PC->ClientPlayCameraShake(Cast<AFPSCharacter>(Cast<AFPSCharacter>(GetOwner()))->FireCamShake);
			}
		}
	}
	MulticastPlayAnim();
}

bool UWeaponRifeMeshComponent::ServerFire_Validate()
{
	return true;
}

void UWeaponRifeMeshComponent::MulticastPlayAnim_Implementation()
{
	if (AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwner())) {
		if (Player->ThirdFireAnimation) {
			UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(Player->ThirdFireAnimation, "Arms", 0.0f);
			}
		}
		if (Player->FireParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Player->FireParticle, GetSocketLocation("Muzzle"));
		}
	}
}

void UWeaponRifeMeshComponent::BeginFire()
{
	//没有子弹，则不会开枪。
	if (BulletCount <= 0)
	{
		if (FireFailSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireFailSound, GetOwner()->GetActorLocation());
		}
		return;
	}

	ServerFire();    //client 不会执行这个函数，而是由服务器执行。
					 // try and fire a projectile
	BulletCount -= 1;
	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
	}

	if (AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwner())) {
		if (Player->FireAnimation) {
			UAnimInstance* AnimInstance = Player->GetMesh1P()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(Player->FireAnimation, "Arms", 0.0f);
			}
		}
	}
}

