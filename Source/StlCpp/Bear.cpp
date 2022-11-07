// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"
#include "BearAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BearAnimInstance.h"
#include "BrainComponent.h"
#include "Kismet/kismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "BearHealthBarWidget.h"

// Sets default values
ABear::ABear()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MaxHP = 30.f;
	CurrentHP = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	IsAttacking = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Game/Bear/Enemy_Bear.Enemy_Bear"));
	if (SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->SetCapsuleRadius(88.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Game/Bear/Animations/ABP_Bear.ABP_Bear_C"));

	if (Anim.Succeeded())
	{
		GetMesh()->SetAnimClass(Anim.Class);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	AIControllerClass = ABearAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//// Pawnsensing

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 500.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	////// Attack Montage Set

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Attack1(TEXT("/Game/Bear/Animations/Attack1.Attack1"));
	if (AM_Attack1.Succeeded())
	{
		Attack1 = AM_Attack1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Attack2(TEXT("/Game/Bear/Animations/Attack2.Attack2"));
	if (AM_Attack2.Succeeded())
	{
		Attack2 = AM_Attack2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Attack3(TEXT("/Game/Bear/Animations/Attack3.Attack3"));
	if (AM_Attack3.Succeeded())
	{
		Attack3 = AM_Attack3.Object;
	}

	HpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpWidget"));
	HpWidget->SetupAttachment(RootComponent);
	HpWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget>HpBarWidget(TEXT("/Game/UI/WB_BearHealthBar.WB_BearHealthBar_C"));
	if (HpBarWidget.Succeeded())
	{
		HpWidget->SetWidgetClass(HpBarWidget.Class);
		HpWidget->SetDrawSize(FVector2D(150.f, 15.f));
	}
}

void ABear::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Bear_Anim = Cast<UBearAnimInstance>(GetMesh()->GetAnimInstance());


	PawnSensing->OnSeePawn.AddDynamic(this, &ABear::SetTarget);
	Bear_Anim->OnMontageEnded.AddDynamic(this, &ABear::OnAttackMontageEnded);

	HpWidget->InitWidget();
	UBearHealthBarWidget* MyHealthWidget = Cast<UBearHealthBarWidget>(HpWidget->GetUserWidgetObject());
	if (MyHealthWidget)
	{
		MyHealthWidget->BindBear(this);
	}

}

// Called when the game starts or when spawned
void ABear::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABear::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHP =FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);
	OnBearHealthChanged.Broadcast();

	if (CurrentHP <= 0)
	{
		Die();
	}

	return 0.f;
}

void ABear::SetTarget(APawn* Pawn)
{
	//TODO if(!TargetPawn)  Targetpawn =Pawn;
	if (TargetPawn == nullptr)
	{
		TargetPawn = Pawn;
		ABearAIController* AIController = Cast<ABearAIController>(GetController());
		if (AIController)
		{
			AIController->SetTarget(Pawn);
		}
	}
	//TODO AICotnroller Target Set;
}

void ABear::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
	IsAttacking = false;
}

void ABear::PlayAttackMontage()
{
	if (!Bear_Anim->IsAnyMontagePlaying())
	{
		IsAttacking = true;

		switch (FMath::RandRange(0, 2))
		{
		case 0: Bear_Anim->Montage_Play(Attack1);
			break;
		case 1: Bear_Anim->Montage_Play(Attack2);
			break;
		case 2: Bear_Anim->Montage_Play(Attack3);
			break;

		default:
			break;
		}
	}
	//Bear_Anim->Montage_Play()
}

void ABear::AttackCheck()
{
	FVector AttackRight = GetMesh()->GetSocketLocation(TEXT("Attack_Right"));
	FVector AttackLeft = GetMesh()->GetSocketLocation(TEXT("Attack_Left"));
	UAnimMontage* CurrentMontage = Bear_Anim->GetCurrentActiveMontage();
	/*TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectType;
	TraceObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));*/
	float AttackRadius = 50.f;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	//TArray<AActor*> OutActors;
	TArray<FHitResult> HitArray;
	bool Hit = false;
	

	//ECC_GameTraceChannel2
	if (CurrentMontage == Attack1)
	{
	/*	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), 
			AttackRight,
			AttackRadius,
			TraceObjectType,
			nullptr, 
			IgnoreActors, 
			OutActors);
		DrawDebugSphere(GetWorld(), AttackRight, AttackRadius, 12, FColor::Red, true, 2.f);*/
		Hit= UKismetSystemLibrary::SphereTraceMulti(GetWorld(), AttackRight, AttackRight, AttackRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, IgnoreActors, EDrawDebugTrace::ForDuration, HitArray, true);
	}
	else if (CurrentMontage == Attack2)
	{
		/*UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
			AttackLeft,
			AttackRadius,
			TraceObjectType,
			nullptr,
			IgnoreActors,
			OutActors);
		DrawDebugSphere(GetWorld(), AttackRight, AttackRadius, 12, FColor::Red, true, 2.f);*/
		Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), AttackLeft, AttackLeft, AttackRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, IgnoreActors, EDrawDebugTrace::ForDuration, HitArray, true);
	}
	else if (CurrentMontage == Attack3)
	{
		//Todo
		Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), AttackLeft, AttackRight, AttackRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, IgnoreActors, EDrawDebugTrace::ForDuration, HitArray, true);
	}
	else
	{

	}
	if (Hit)
	{
		for (const auto& HitResult : HitArray)
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10.f, GetController(), this, nullptr);
		}
	}
	//for (AActor* OverlapActor : OutActors)
	//{	
	//	UGameplayStatics::ApplyDamage(OverlapActor, 10.f, GetController(), this, nullptr);
	//	UE_LOG(LogTemp, Warning, TEXT("OverlappedActor:  %s"), *OverlapActor->GetName());
	//}
}

void ABear::Die()
{
	//TODO
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->StopMovementImmediately();

	Bear_Anim->StopAllMontages(0.f);
	OnBearDied.Broadcast();
	Cast<AAIController>(GetController())->BrainComponent->StopLogic(TEXT("Death"));

	FTimerHandle DestroyHandle;
	float Delaytime = 3.f;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), Delaytime, false);



}

float ABear::GetHpRatio()
{
	return (CurrentHP/MaxHP);
}
