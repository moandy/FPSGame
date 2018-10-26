// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "Engine/Engine.h"
#include "FPSAIGuard.h"


// Sets default values
AFPSObjectActor::AFPSObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	SphereComp->SetupAttachment(MeshComp);


	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffects();
}

void AFPSObjectActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// Called every frame
void AFPSObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSObjectActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	PlayEffects();

	if (Role == ROLE_Authority) {      //只删除服务器上的Object，然后通过复制删除其他客户端的复制品
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, FString::FromInt(Role) + "ROLE," + OtherActor->GetFName().ToString());        //DEBUG Message
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter) {
			MyCharacter->bIsCarryingObjective = true;

			Destroy();
			
		}
	}
	
}
