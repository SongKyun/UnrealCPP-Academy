// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "StlGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FStlCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FStlCharacterData(): Level(1), MaxHP(100.f), Attack(10.f),NextEXP(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 NextEXP;

};
UCLASS()
class STLCPP_API UStlGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UStlGameInstance();

	virtual void Init() override;
	FStlCharacterData* GetStlCharData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* StlCharacterTable;

};
