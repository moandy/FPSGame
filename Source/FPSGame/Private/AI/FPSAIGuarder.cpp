// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuarder.h"
#include "FPSGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "WidgetComponent.h"




AFPSAIGuarder::AFPSAIGuarder()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);
	WidgetComp->RelativeLocation = FVector(0.f, 0.f, 90.f);

	GuardState = EAIGuardState::Idle;

	GetCapsuleComponent()->SetCapsuleRadius(38.f);
	GetMesh()->RelativeLocation = FVector(0.f, 0.f, -60.f);
	GetMesh()->RelativeRotation = FRotator(270.f, 0.f, 0.f);

	

	PawnSensingComp->HearingThreshold = 300.f;
	PawnSensingComp->LOSHearingThreshold = 1000.f;
	PawnSensingComp->SightRadius = 1400.f;
	PawnSensingComp->SensingInterval = 0.2f;
	PawnSensingComp->SetIsReplicated(true);

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuarder::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuarder::OnNoiseHeard);

	this->SetReplicates(true);
	SetReplicateMovement(true);

	//SetHealth(100.f);

	bSeenPawn = false;
}

void AFPSAIGuarder::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	if (bPatrol) {
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuarder::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12.f, FColor::Yellow, false, 10.0f);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnPawnSeen"), true);
	bSeenPawn = true;

	SetGuardState(EAIGuardState::Alerted);

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), SeenPawn);
	//
	FVector Delta = GetActorLocation() - SeenPawn->GetActorLocation();
	float DistanceToGoal = Delta.Size();
	//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, FString::SanitizeFloat(DistanceToGoal) + "NO");
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

void AFPSAIGuarder::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.f, 12.f, FColor::Green, false, 10.0f);
	//设置转向
	FVector Diretion = Location - GetActorLocation();
	Diretion.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Diretion).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;
	SetActorRotation(NewLookAt);

	//定时转向
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuarder::ResetOrientation, 2.0f);

	SetGuardState(EAIGuardState::Suspicious);

	AController* Controller = GetController();
	if (Controller) Controller->StopMovement();

}

void AFPSAIGuarder::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIGuardState::Idle);

	if (bPatrol) {
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuarder::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}


// TODO
void AFPSAIGuarder::SetGuardState(EAIGuardState newState)
{
	if (GuardState == newState) return;
	
	GuardState = newState;

	//OnRep_GuardState();
}

void AFPSAIGuarder::MoveToNextPatrolPoint()
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

void AFPSAIGuarder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSeenPawn) {
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
}

void AFPSAIGuarder::OnHealthHurt(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	OnHealthChanged(tHealth);
	if (tHealth <= 0.0f) {
		Destroy();
	}
}


void AFPSAIGuarder::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuarder, GuardState);

	//DOREPLIFETIME(AFPSAIGuarder, Health);
}