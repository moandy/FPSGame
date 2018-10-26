// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GamePlay/FPSPlayerController.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/ChatUI/FPSChatSystemWidget.h"
#include "UI/ChatUI/FPSMessageWidget.h"
#include "CharacterUI/FPSCharacterWidget.h"
#include "FPSPlayerState.h"
#include "WidgetBlueprintLibrary.h"
#include "Component/WeaponBaseMeshComponent.h"
#include "Component/WeaponRifeMeshComponent.h"
#include "FPSHealthComponent.h"


AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->RelativeLocation = FVector(0, 0, BaseEyeHeight); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->RelativeRotation = FRotator(2.0f, -15.0f, 5.0f);
	Mesh1PComponent->RelativeLocation = FVector(0, 0, -160.0f);
	

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<UWeaponRifeMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
	

	GunSocketName = FName("WeaponSocket");

	// ThirdGunMesh
	ThirdGunMeshComponent = CreateDefaultSubobject<UWeaponRifeMeshComponent>(TEXT("ThirdGunMeshComp"));
	ThirdGunMeshComponent->SetupAttachment(GetMesh(), GunSocketName);
	
	//create a noise emitter component
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	// HealthComp
	HealthComp = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("HealthComp"));

	BulletSum = BulletCount = 20;

	//Health for Character
// 	HealthSum = Health = 100.f;
// 	HealthRate = 1.f;

	bDied = false;
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);


	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::BeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::StopFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("MouseShow", IE_Pressed, this, &AFPSCharacter::MouseShow);
	//PlayerInputComponent->BindAction("MouseShow", IE_Released, this, &AFPSCharacter::MouseNotShow);
}


void AFPSCharacter::Tick(float DeltaTime)   //cs都会执行这个函数
{
	Super::Tick(DeltaTime);
	if (!IsLocallyControlled()) {
		FRotator NewRot = CameraComponent->RelativeRotation;
		NewRot.Pitch = RemoteViewPitch * 360.f / 255.f;         //从客户端获取Y视角
		CameraComponent->SetRelativeRotation(NewRot);   //服务器更新视角

	}

}


void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Mesh1PComponent->SetOnlyOwnerSee(true);
	GunMeshComponent->SetOnlyOwnerSee(true);

	ThirdGunMeshComponent->SetOwnerNoSee(true);
	ThirdGunMeshComponent->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetOnlyOwnerSee(false);

	// Add the HealthWidget and AddtoViewport
	APlayerController* PC = Cast<APlayerController>(GetController());
	UClass* BPClass = LoadClass<UFPSCharacterWidget>(NULL, TEXT("WidgetBlueprint'/Game/Blueprints/UI/CharacterUI/BP_FPSCharacterWidget.BP_FPSCharacterWidget_C'"));
	if (PC&&IsLocallyControlled()&&BPClass) {
		HealthWidget = CreateWidget<UFPSCharacterWidget>(PC, BPClass);
		HealthWidget->AddToViewport();
	}

	HealthComp->OnHealthChanged.AddDynamic(this, &AFPSCharacter::OnHealthChanged);
}



void AFPSCharacter::OnHealthChanged(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (tHealth <= 0.0f && !bDied) {
		// Remove the HealthWidget
		ClientRemoveWidget();
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(5.f);
		bDied = true;
	}
}


void AFPSCharacter::ClientRemoveWidget_Implementation()
{
	if (HealthWidget) {
		HealthWidget->RemoveFromViewport();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, "WidgetRemove");
	}
}


void AFPSCharacter::BeginFire()
{
	if (GunMeshComponent) {
		//CurrentWeapon->Fire();
		GunMeshComponent->BeginFire();
	}
}

void AFPSCharacter::StopFire()
{
	if (GunMeshComponent) {
		GunMeshComponent->StopFire();
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSCharacter::MouseShow()
{
	AFPSPlayerController* PC = Cast<AFPSPlayerController>(GetController());
	if (PC) {
		PC->bShowMouseCursor = PC->bShowMouseCursor?false:true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, "MouseShow");
}



void AFPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, bIsCarryingObjective);
	//DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, bDied);

	//DOREPLIFETIME(AFPSCharacter, CurrentWeapon);
}




// void AFPSCharacter::ServerFire_Implementation()
// {
// 	if (ProjectileClass)
// 	{
// 		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
// 		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");
// 
// 		//Set Spawn Collision Handling Override
// 		FActorSpawnParameters ActorSpawnParams;
// 		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
// 		ActorSpawnParams.Instigator = this;			//hello
// 		ActorSpawnParams.Owner = this;
// 
// 		//GetWorld()->LineTraceSingleByProfile()
// 													// spawn the projectile at the muzzle
// 		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
// 
// 		if (APlayerController* PC = Cast<APlayerController>(GetController())) {
// 			if (FireCamShake) {
// 				PC->ClientPlayCameraShake(FireCamShake);
// 			}
// 		}
// 	}
// 	MulticastPlayAnim();
// }


// bool AFPSCharacter::ServerFire_Validate()
// {
// 	return true;
// }


// void AFPSCharacter::MulticastPlayAnim_Implementation()
// {
// 	if (ThirdFireAnimation) {
// 		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
// 		if (AnimInstance)
// 		{
// 			AnimInstance->PlaySlotAnimationAsDynamicMontage(ThirdFireAnimation, "Arms", 0.0f);
// 		}
// 	}
// 
// 	if (FireParticle)
// 	{
// 		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, GunMeshComponent->GetSocketLocation("Muzzle"));
// 	}
// }

// void AFPSCharacter::Fire()
// {
// 	//没有子弹，则不会开枪。
// 	if (BulletCount <= 0)
// 	{
// 		if (FireFailSound)
// 		{
// 			UGameplayStatics::PlaySoundAtLocation(this, FireFailSound, GetActorLocation());
// 		}		
// 		return;
// 	}
// 
// 	ServerFire();    //client 不会执行这个函数，而是由服务器执行。
// 	// try and fire a projectile
// 	BulletCount -= 1;
// 	// try and play the sound if specified
// 	if (FireSound)
// 	{
// 		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
// 	}
// 
// 	// try and play a firing animation if specified
// 	if (FireAnimation)
// 	{
// 		// Get the animation object for the arms mesh
// 		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
// 		if (AnimInstance)
// 		{
// 			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
// 		}
// 	}
// 	
// }

// void AFPSCharacter::OnRep_Health()
// {
// 	HealthRate = Health / HealthSum;
// }

// void AFPSCharacter::OnHealthHurt(float Damage)
// {
// 	if (Role == ROLE_Authority) {
// 		Health -= Damage;
// 		if (Health <= 0.0f) {
// 			// Remove the HealthWidget
// 			ClientRemoveWidget();
// 			GetMovementComponent()->StopMovementImmediately();
// 			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 			DetachFromControllerPendingDestroy();
// 			SetLifeSpan(5.f);
// 			bDied = true;
// 		}
// 	}
// }