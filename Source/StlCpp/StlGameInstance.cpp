// Fill out your copyright notice in the Description page of Project Settings.


#include "StlGameInstance.h"

UStlGameInstance::UStlGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Game/DataTable/DT_CharacterData.DT_CharacterData"));
	if (DT_CharacterData.Succeeded())
	{
		StlCharacterTable = DT_CharacterData.Object;
	}
	check(StlCharacterTable->GetRowMap().Num() > 0);
}

void UStlGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("EXP: %d"), GetStlCharData(5)->NextEXP);
}

FStlCharacterData* UStlGameInstance::GetStlCharData(int32 Level)
{
	return StlCharacterTable->FindRow<FStlCharacterData>(*FString::FromInt(Level), TEXT(""));
}