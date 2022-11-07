// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class STLCPP_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

	UPROPERTY(VisibleAnywhere, Category = ItemBox)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = ItemBox)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = ItemBox)
	TSubclassOf<class ABaseWeapon> WeaponItemClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, 
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp, 
							int32 OtherBodyIndex, 
							bool bFromSweep, 
							const FHitResult& SweepResult);
};
