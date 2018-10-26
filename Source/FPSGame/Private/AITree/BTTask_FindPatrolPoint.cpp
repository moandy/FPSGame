// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPoint.h"
#include "FPSAIController.h"
#include "FPSTargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTNode.h"
#include "UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"





EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AFPSAIController* MyAIPC = Cast<AFPSAIController>(OwnerComp.GetAIOwner());
	if (MyAIPC == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	TArray<AActor*> PatrolPoints;
	UGameplayStatics::GetAllActorsOfClass(MyAIPC, AFPSTargetPoint::StaticClass(), PatrolPoints);

	if (MyAIPC->GetBlackboardComponent()) {
		MyAIPC->GetBlackboardComponent()->SetValueAsObject(MyAIPC->PatrolPoint, PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)]);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
