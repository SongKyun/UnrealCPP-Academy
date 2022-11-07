// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BearHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class STLCPP_API UBearHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	void BindBear(class ABear* MyBear);
	void UpdateHp();
	class ABear* MyBear;

	UPROPERTY()
		class UProgressBar* HpBar;

};
