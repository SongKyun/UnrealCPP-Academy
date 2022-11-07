// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "StlCppCharacter.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SM_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

	SM_Door->SetupAttachment(RootComponent);
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.f, 80.f, 40.f));
	Trigger->SetBoxExtent(FVector(100.f, 80.f, 40.f));
	static ConstructorHelpers::FObjectFinder <UStaticMesh>Door(TEXT("/Game/StaticMesh/SM_GATE.SM_GATE"));

	if (Door.Succeeded())
	{
		SM_Door->SetStaticMesh(Door.Object);
	}
	
}

void ADoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnCharacterBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnCharacterEndOverlap);

}
// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentRot.Equals(TargetRot))
	{
		CurrentRot = FMath::RInterpConstantTo(CurrentRot, TargetRot, DeltaTime, 45.f);
		SM_Door->SetRelativeRotation(CurrentRot);
	}

}

void ADoor::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast <AStlCppCharacter>(OtherActor);
	if (Character)
	{
		FVector DoorDir = GetActorForwardVector();
		FVector CharDir = GetActorLocation() - OtherActor->GetActorLocation();
		CharDir.Normalize();
		DoorDir.Normalize();

		//Direction = CharDir.Dot(DoorDir);
		Direction = FVector::DotProduct(CharDir, DoorDir);

		TargetRot = (Direction > 0.f) ? FRotator(0.f, -90.f, 0.f) : FRotator(0.f, 90.f, 0.f);
		CurrentRot = SM_Door->GetRelativeRotation();

	}
}


void ADoor::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TargetRot = FRotator::ZeroRotator;
	CurrentRot = SM_Door->GetRelativeRotation();
}