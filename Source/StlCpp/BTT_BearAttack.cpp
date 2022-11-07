// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BearAttack.h"
#include "BearAIController.h"
#include "Bear.h"

UBTT_BearAttack::UBTT_BearAttack()
{
	NodeName = (TEXT("Bear Attack"));
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_BearAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// TODO 
	// 1. 공격 실행   Bear -> Attack();
	// 2. finish 호출 불가
	// 3. return :: InProgress
	ABear* ControlledBear = Cast<ABear>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControlledBear)
	{
		ControlledBear->PlayAttackMontage();
		ControlledBear->OnAttackEnd.AddLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
