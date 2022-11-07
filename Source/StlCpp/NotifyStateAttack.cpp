// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStateAttack.h"
#include "StlCppCharacter.h"

void UNotifyStateAttack:: NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AStlCppCharacter* Character = Cast<AStlCppCharacter>(MeshComp->GetOwner());
	
	if (Character)
	{
		Character->bIsAttackCheck = true;
		Character->ActorsToIgnore.Add(Character);
	}
}
void UNotifyStateAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AStlCppCharacter* Character = Cast<AStlCppCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->bIsAttackCheck = false;
		Character->ActorsToIgnore.Empty();
	}
}