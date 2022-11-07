// Fill out your copyright notice in the Description page of Project Settings.


#include "BearAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StlCppCharacter.h"

ABearAIController::ABearAIController()
{
	//변경되어야할 코드
	Blackboard = MyBlackboard;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_Data(TEXT("/Game/Bear/AI/BB_Bear.BB_Bear"));
	if (BB_Data.Succeeded())
	{
		BB = BB_Data.Object;
	}

	static ConstructorHelpers::FObjectFinder <UBehaviorTree> BT_Data(TEXT("/Game/Bear/AI/BT_Bear.BT_Bear"));
	if (BT_Data.Succeeded())
	{
		BT = BT_Data.Object;
	}


}
void ABearAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
void ABearAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BB, MyBlackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("Set BlackBoard"));
	}

	if (!RunBehaviorTree(BT))
	{
		UE_LOG(LogTemp, Warning, TEXT("Run BT Error"));
	}
}
void ABearAIController::OnUnPossess() 
{
	Super::OnUnPossess();
}

void ABearAIController::SetTarget(APawn* InPawn)
{
	auto TargetChar = Cast<AStlCppCharacter>(InPawn);
	if (TargetChar)
	{
		Blackboard->SetValueAsObject(TEXT("TargetActor"), TargetChar);
	}
}