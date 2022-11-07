// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "BearAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation()
{
	NodeName = TEXT("FindRandomLocation");
}

EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn == nullptr)  // !Pawn  ,  Pawn.IsValidLowLevel() ::Valid(Pawn)
	{
		return EBTNodeResult::Failed;
	}
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	FNavLocation TargetLoc;

	if (NavSystem->GetRandomPointInNavigableRadius(Pawn->GetActorLocation(), 1000.f, TargetLoc))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RandomLocation"), TargetLoc.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}