// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "BaseWeapon.h"
#include "StlCppCharacter.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	RootComponent = Box;
	Trigger->SetupAttachment(Box);
	Trigger->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	WeaponItemClass = ABaseWeapon::StaticClass();

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Mesh(TEXT("/Game/ItemBox/Meshes/Props/SM_MerchantShip_Storagebox_03.SM_MerchantShip_Storagebox_03"));

	if (SM_Mesh.Succeeded())
	{
		Box->SetStaticMesh(SM_Mesh.Object);
	}


}
void AItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnCharacterOverlap);

 }
// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));

	auto StlCharacter = Cast<AStlCppCharacter>(OtherActor);
	//우리 캐릭터인지 캐스팅
	if (StlCharacter)
	{
		if (StlCharacter->CanGetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponItemClass);
			StlCharacter->AddWeaponToCharacter(NewWeapon);
			Destroy();
		}		
	}
}