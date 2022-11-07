// Fill out your copyright notice in the Description page of Project Settings.


#include "BearAnimInstance.h"
#include "Bear.h"

UBearAnimInstance::UBearAnimInstance()
{
	PawnSpeed = 0.f;
	// TODO :: AttackMontage
}

void UBearAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABear* Bear = Cast<ABear>(TryGetPawnOwner());
	if (::IsValid(Bear))
	{
		PawnSpeed = Bear->GetVelocity().Size();

		Bear->CurrentHP <= 0 ? IsDead = true : IsDead = false;
		
	}
}


void UBearAnimInstance::AnimNotify_Attack()
{
	ABear* Bear = Cast<ABear>(TryGetPawnOwner());
	if (::IsValid(Bear))
	{
		Bear->AttackCheck();
	}
}