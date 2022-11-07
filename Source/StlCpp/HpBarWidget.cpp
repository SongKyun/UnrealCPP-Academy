// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarWidget.h"
#include "StlCharacterStatComponent.h" 
#include "Components/ProgressBar.h"

void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Hp")));
	UpdateHp();
}

void UHpBarWidget::UpdateHp()
{
	if (CharacterStat)
	{	
		HpBar->SetPercent(CharacterStat->GetHpRatio());
	}
}

void UHpBarWidget::BindCharcterStat(class UStlCharacterStatComponent* Statcomp)
{
	if (Statcomp)
	{
		CharacterStat = Statcomp;
		Statcomp->OnCharacterHealthChanged.AddUObject(this, &UHpBarWidget::UpdateHp);
		// 델리게이트를 바인딩

	}
}