// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StlCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterHealthChanged);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STLCPP_API UStlCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStlCharacterStatComponent();

	void SetNewLevel(int32 NewLevel);
	float GetCharacterAttackDamage();
	void SetCurrentHpByDamage(float NewDamage);
	float GetHpRatio();

	FOnCharacterHealthChanged OnCharacterHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	struct FStlCharacterData* CurrentStat= nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess))
	int32 Level;

	UPROPERTY(Transient, VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess))
	float CurrentHP;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
