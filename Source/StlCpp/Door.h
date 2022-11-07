// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StlCpp.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class STLCPP_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float Direction;

	UPROPERTY(VisibleAnywhere)
	FRotator CurrentRot;

	UPROPERTY(VisibleAnywhere)
	FRotator TargetRot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM_Door;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Trigger;

	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
