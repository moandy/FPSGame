// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIChaser.h"
#include "Components/CapsuleComponent.h"
#include "UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSAIController.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/PrimitiveComponent.h"
#include "ConstructorHelpers.h"



AFPSAIChaser::AFPSAIChaser()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);

	GetMesh()->RelativeLocation = FVector(0.f, 0.f, -85.f);
	GetMesh()->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	//PawnSensingComp->HearingThreshold = 300.f;
	//PawnSensingComp->LOSHearingThreshold = 1000.f;
	PawnSensingComp->SightRadius = 1400.f;
	PawnSensingComp->SensingInterval = 0.2f;
	PawnSensingComp->SetIsReplicated(true);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIChaser::OnPawnSeen);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetRelativeLocation(FVector(45.0f, 0.f, 25.f));
	CapsuleComp->SetCapsuleHalfHeight(44.f);
	CapsuleComp->SetCapsuleRadius(22.f);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSAIChaser::OnComponentBeginOverlap);
	CapsuleComp->SetupAttachment(GetCapsuleComponent());

	bDied = false;

}

void AFPSAIChaser::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPawn) {
		if (!PawnSensingComp->CouldSeePawn(TargetPawn)) {
			if (AFPSAIController* MyAIPC = Cast<AFPSAIController>(GetController())) {
				MyAIPC->GetBlackboardComponent()->SetValueAsObject(MyAIPC->TargetActor, nullptr);
				TargetPawn = nullptr;
			}
		}
	}
}

void AFPSAIChaser::BeginPlay()
{
	Super::BeginPlay();
	
	if (AFPSAIController* MyAIPC = Cast<AFPSAIController>(GetController())) {
		check(FPSBehaviorTree);
		MyAIPC->RunBehaviorTree(FPSBehaviorTree);
	}

}

// void AFPSAIChaser::OnHealthHurt(float Damage)
// {
// 	if (Role == ROLE_Authority) {
// 		if (Health - Damage < 0.f) Health = 0.f;
// 		else Health -= Damage;
// 		if (Health <= 0.f) {
// 			CapsuleComp->OnComponentBeginOverlap.RemoveDynamic(this, &AFPSAIChaser::OnComponentBeginOverlap);
// 			DetachFromControllerPendingDestroy();
// 			SetLifeSpan(2.f);
// 			bDied = true;
// 		}
// 	}
// }


void AFPSAIChaser::OnPawnSeen(APawn* SeenPawn)
{
	if (AFPSAIController* MyAIPC = Cast<AFPSAIController>(GetController())) {
		MyAIPC->GetBlackboardComponent()->SetValueAsObject(MyAIPC->TargetActor, SeenPawn);
		TargetPawn = SeenPawn;
	}
}

void AFPSAIChaser::OnHealthHurt(UFPSHealthComponent* OwningHealthComp, float tHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (tHealth <= 0.f) {
		CapsuleComp->OnComponentBeginOverlap.RemoveDynamic(this, &AFPSAIChaser::OnComponentBeginOverlap);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(2.f);
		bDied = true;
	}
}

void AFPSAIChaser::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AFPSCharacter* MyPlayer = Cast<AFPSCharacter>(OtherActor)) {
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->CompleteMission(MyPlayer, false);
		}
	}
}

void AFPSAIChaser::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIChaser, bDied);
}

