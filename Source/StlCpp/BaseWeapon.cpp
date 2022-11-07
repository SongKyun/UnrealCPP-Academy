// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = SK_Weapon;
	SK_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SK_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder <USkeletalMesh>SK_Mesh(TEXT("/Game/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));

	if (SK_Mesh.Succeeded())
	{
		SK_Weapon->SetSkeletalMesh(SK_Mesh.Object);
	}
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

