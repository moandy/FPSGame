// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"

#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class UPawnNoiseEmitterComponent;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UWeaponBaseMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UWeaponBaseMeshComponent* ThirdGunMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class UFPSHealthComponent* HealthComp;
public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		TSubclassOf<UCameraShake> FireCamShake;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* FireFailSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		UAnimSequence* ThirdFireAnimation;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Gameplay)
	bool bIsCarryingObjective;

	UPROPERTY(VisibleDefaultsOnly, Category = GamePlay)
		FName GunSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UParticleSystem* FireParticle;

	UPROPERTY(BlueprintReadWrite, Category = Widget)
		class UFPSCharacterWidget* HealthWidget;

protected:
	
	
// 
// 	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
// 		void MulticastMessage(const FString& PlayerName, const FString& Message);

// 	UFUNCTION(NetMulticast, Reliable)
// 		void MulticastPlayAnim();

	

// 	UFUNCTION(Server, Reliable, WithValidation)
// 	void ServerFire();
// 
// 	/** Fires a projectile. */
// 	void Fire();

	

	void BeginFire();

	void StopFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	void MouseShow();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

	UWeaponBaseMeshComponent* GetThirdGunMesh() const { return ThirdGunMeshComponent; }

	UWeaponBaseMeshComponent* GetGunMesh() const { return GunMeshComponent; }

public:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

/*	void OnHealthHurt(float Damage);*/

	UFUNCTION(BlueprintCallable)
		void OnHealthChanged(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

// 	UFUNCTION()
// 		void OnRep_Health();


	UFUNCTION(Client, Reliable, Category = RPC)
	void ClientRemoveWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void CreateChat(APlayerController* PlayerController);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlay)
		int32 BulletCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GamePlay)
		int32 BulletSum;

// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlay)
// 		float HealthSum;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = GamePlay)
// 		float Health;
// 
// 	UPROPERTY(BlueprintReadOnly, Category = GamePlay)
// 		float HealthRate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GamePlay)
		bool bDied;

};
