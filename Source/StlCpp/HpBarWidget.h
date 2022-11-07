// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "Blueprint/UserWidget.h"
#include "HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class STLCPP_API UHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;
	void BindCharcterStat(class UStlCharacterStatComponent* Statcomp);
	void UpdateHp();
	class UStlCharacterStatComponent* CharacterStat;

	UPROPERTY()
	class UProgressBar* HpBar;

};
