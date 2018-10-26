// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "NoExportTypes.h"
#include "Engine/Engine.h"
#include "FPSGameMode.h"
#include "UnrealNetwork.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "NavigationData.h"
#include "NavigationSystemTypes.h"
#include "TimerManager.h"
#include "FPSHealthComponent.h"


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	GuardState = EAIState::Idle;
	
	this->SetReplicates(true);
	SetReplicateMovement(true);

	HealthComp = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("HealthComp"));


	//health = 200.f;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	if (bPatrol) {
		MoveToNextPatrolPoint();
	}

	HealthComp->OnHealthChanged.AddDynamic(this, &AFPSAIGuard::OnHealthHurt);
}


//  设置AI看到人物就去追， 追到就结束。
void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12.f, FColor::Yellow, false, 10.0f);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnPawnSeen"), true);
	SetGuardState(EAIState::Alerted);

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), SeenPawn);
	//
	FVector Delta = GetActorLocation() - SeenPawn->GetActorLocation();
	float DistanceToGoal = Delta.Size();
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, FString::SanitizeFloat(DistanceToGoal) + "NO");
	if (DistanceToGoal < 100) {
		APawn* PC = Cast<APawn>(SeenPawn);
		if (PC == nullptr) return;
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->CompleteMission(PC, false);
		}

		AController* Controller = GetController();
		if (Controller) Controller->StopMovement();
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::SanitizeFloat(DistanceToGoal) + "YES");
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{	

	DrawDebugSphere(GetWorld(), Location, 32.f, 12.f, FColor::Green, false, 10.0f);

	FVector Diretion = Location - GetActorLocation();
	Diretion.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Diretion).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 2.0f);

	SetGuardState(EAIState::Suspicious);

	AController* Controller = GetController();
	if (Controller) Controller->StopMovement();

}

void AFPSAIGuard::ResetOrientation()
{
// 	if (GuardState == EAIState::Alerted) {
// 		return;
// 	}
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);

	if (bPatrol) {
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}


void AFPSAIGuard::SetGuardState(EAIState newState)
{
	if (GuardState == newState) return;
	GuardState = newState;

	OnRep_GuardState();
	//OnStateChanged(GuardState);
}


void AFPSAIGuard::OnHealthHurt(class UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::SanitizeFloat(tHealth) + "NO");
	OnHealthChanged(tHealth);
	if (tHealth <= 0.0f) {
		Destroy();
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint) {
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();
		//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, GetFName().ToString() + FString::SanitizeFloat(DistanceToGoal) + "NO");
		if (DistanceToGoal < 100) {
			MoveToNextPatrolPoint();
			//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::SanitizeFloat(DistanceToGoal) + "YES");
		}
	}
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{	
	if (CurrentPatrolPoint != nullptr) {
		for (int i = 0; i < PatrolPoint.Num(); i++) {
			if (CurrentPatrolPoint == PatrolPoint[i] && i != PatrolPoint.Num() - 1) {
				CurrentPatrolPoint = PatrolPoint[i + 1];
				break;
			}
			else if (i == PatrolPoint.Num() - 1) {
				CurrentPatrolPoint = PatrolPoint[0];
				break;
			}
		}
	}
	else {
		CurrentPatrolPoint = PatrolPoint[0];
	}

	if (GetController() != nullptr) 
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
	}
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);

	//DOREPLIFETIME(AFPSAIGuard, health);
}


// void AFPSAIGuard::OnRep_Health()
// {
// 	GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, GetFName().ToString() + FString::SanitizeFloat(health));
// 	OnHealthChanged(health);
// }
// 
// void AFPSAIGuard::OnHealthHurt(float Damage)
// {
// 	if (Role == ROLE_Authority) {
// 		health -= Damage;
// 		if (health <= 0.0f) {
// 			Destroy();
// 		}
// 	}
// 		
// //	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, GetFName().ToString() + FString::SanitizeFloat(health));
// // 	if (Role == ROLE_Authority) {
// // 		OnRep_Health();
// // 	}
// 	
// }