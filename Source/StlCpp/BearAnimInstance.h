// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "Animation/AnimInstance.h"
#include "BearAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STLCPP_API UBearAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBearAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UFUNCTION()
	void AnimNotify_Attack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bear, Meta = (AllowPrivateAccess))
	float PawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bear, Meta = (AllowPrivateAccess))
	bool IsDead;


};
