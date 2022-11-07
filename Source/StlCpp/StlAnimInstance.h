// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "Animation/AnimInstance.h"
#include "StlAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);

/**
 * 
 */
UCLASS()
class STLCPP_API UStlAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UStlAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FOnNextAttackCheckDelegate OnNextAttackCheck;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float PawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

};
