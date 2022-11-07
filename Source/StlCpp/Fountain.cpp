// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	RotationSpeed = 1.f;

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.f, 0.f, 130.f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Body(TEXT("/Game/Fountain/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Water(TEXT("/Game/Fountain/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	if (SM_Water.Succeeded())
	{
		Water->SetStaticMesh(SM_Water.Object);
	}


}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, (RotationSpeed * DeltaTime), 0.f));

}

