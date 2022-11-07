// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Bear.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnCount = 0;
	MaxCount = 5;
	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Area->SetupAttachment(Root);
	SpawnEnemyClass = ABear::StaticClass();

	ElapsedTime = 0.f;

}
void ASpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ASpawner::SpawnEnemy, 5.f, true);
}
// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*ElapsedTime += DeltaTime;
	if (ElapsedTime >= 5.0f)
	{
		SpawnEnemy();
	}*/

}

void ASpawner::SpawnEnemy()
{
	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), Area->GetScaledBoxExtent());
	if (SpawnCount < MaxCount)
	{
		auto SpawnedEnemy = GetWorld()->SpawnActor<ABear>(RandomLocation, FRotator::ZeroRotator);
		
		if (SpawnedEnemy)
		{
			SpawnedEnemy->OnBearDied.AddLambda([this]()->void {
				SpawnCount--;
				});
			SpawnCount++;
			ElapsedTime = 0.f;
		}
	}
	
}