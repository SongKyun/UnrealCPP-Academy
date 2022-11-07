// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "GameFramework/Character.h"
#include "StlCppCharacter.generated.h"

UCLASS()
class STLCPP_API AStlCppCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStlCppCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UStlCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HpWidget;

	UPROPERTY()
	class UStlAnimInstance* StlAnim;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void Attack();

	UFUNCTION()
	void AddWeaponToCharacter(AActor* Weapon);
	UFUNCTION()
	bool CanGetWeapon();
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ABaseWeapon* CurrentWeapon;
	UPROPERTY()
	bool bIsAttackCheck;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;


private:
	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess))
	bool bIsAttacking;
	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess))
	bool bCanNextCombo;
	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess))
	bool bIsComboInput;
	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess))
	int32 CurrentCombo;
	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess))
	int32 MaxCombo;

public:
	void AttackStartCombo();
	void AttackEndCombo();
	void AttackCheck();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};
