// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "GameFramework/Character.h"
#include "Bear.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBearDieDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBearHealthChanged);

UCLASS()
class STLCPP_API ABear : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UBearAnimInstance* Bear_Anim;

	UPROPERTY(VisibleAnywhere)
	float CurrentHP;

	UPROPERTY(VisibleAnywhere)
	float MaxHP;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* HpWidget;

	FOnBearDieDelegate OnBearDied;
	FOnAttackEndDelegate OnAttackEnd;
	FOnBearHealthChanged OnBearHealthChanged;

	UPROPERTY()
	UAnimMontage* Attack1;
	UPROPERTY()
	UAnimMontage* Attack2;
	UPROPERTY()
	UAnimMontage* Attack3;
	UPROPERTY()
	bool IsAttacking;

	UFUNCTION()
	void SetTarget(APawn* Pawn);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void PlayAttackMontage();

	UFUNCTION()
	void AttackCheck();

	UFUNCTION()
	void Die();

	UFUNCTION()
		float GetHpRatio();
	

	UPROPERTY()
	APawn* TargetPawn;


};
