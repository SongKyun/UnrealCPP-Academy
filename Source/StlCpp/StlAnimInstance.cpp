// Fill out your copyright notice in the Description page of Project Settings.


#include "StlAnimInstance.h"

UStlAnimInstance::UStlAnimInstance()
{
	PawnSpeed = 0.f;
	bIsInAir = false;

	ConstructorHelpers::FObjectFinder<UAnimMontage>AM_Attack(TEXT("/Game/Animations/AM_Attack.AM_Attack"));
	if (AM_Attack.Succeeded())
	{
		AttackMontage = AM_Attack.Object;
	}
}

void UStlAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		PawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			bIsInAir = Character->GetCharacterMovement()->IsFalling();
		}
	}
}

void UStlAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage);
	}
}
void UStlAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	FName SectionName = (*(FString(TEXT("Attack")) + FString::FromInt(NewSection)));
	Montage_JumpToSection(SectionName, AttackMontage);
}


void UStlAnimInstance::AnimNotify_NextAttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Check Notify"));
	OnNextAttackCheck.Broadcast();
}