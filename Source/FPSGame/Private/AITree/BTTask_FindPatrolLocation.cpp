// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolLocation.h"
#include "FPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSTargetPoint.h"
#include "NavigationSystem.h"


EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AFPSAIController* MyAIPC = Cast<AFPSAIController>(OwnerComp.GetAIOwner());
	if (MyAIPC == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = MyAIPC->GetBlackboardComponent();
	if (Blackboard) {
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Blackboard1"), true);
		if (AFPSTargetPoint* PatrolPoint = Cast<AFPSTargetPoint>(Blackboard->GetValueAsObject(MyAIPC->PatrolPoint))) {
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Blackboard2"), true);
			FVector Location = PatrolPoint->GetActorLocation();
			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomPointInNavigableRadius(MyAIPC, Location, RandomLocation, 700.f);
			Blackboard->SetValueAsVector(MyAIPC->PatrolDestination, RandomLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
