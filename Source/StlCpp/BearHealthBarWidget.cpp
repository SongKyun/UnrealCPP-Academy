// Fill out your copyright notice in the Description page of Project Settings.


#include "BearHealthBarWidget.h"
#include "Bear.h"
#include "Components/ProgressBar.h"

void UBearHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	UpdateHp();
}

void UBearHealthBarWidget::BindBear(ABear* OwnerBear)
{
	if (OwnerBear)
	{
		MyBear = OwnerBear;
		MyBear->OnBearHealthChanged.AddUObject(this, &UBearHealthBarWidget::UpdateHp);
	}
}

void UBearHealthBarWidget::UpdateHp()
{
	if (MyBear)
	{
		HpBar->SetPercent(MyBear->GetHpRatio());
	}
}