// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class STLCPP_API UBTT_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_FindRandomLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
