// Fill out your copyright notice in the Description page of Project Settings.


#include "StlCppCharacter.h"
#include "StlAnimInstance.h"
#include "BaseWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "StlCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "HpBarWidget.h"

// Sets default values
AStlCppCharacter::AStlCppCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("StlCharacter"));

	//SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden'

	//////////////////////////// 메시 관련 설정 //////////////////////////////////////////////////
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SK_Body(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden"));

	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Character(TEXT("/Game/Animations/ABP_Character.ABP_Character_C"));
	if (Anim_Character.Succeeded())
	{
		GetMesh()->SetAnimClass(Anim_Character.Class);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////// 카메라 세팅 ////////////////////////////
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 450.f;
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch= false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw  = false;

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	////////////////////////////////////////////////////////////////

	bIsAttackCheck = false;
	bIsAttacking = false;
	MaxCombo = 4;
	CurrentCombo = 0;

	CharacterStat = CreateDefaultSubobject<UStlCharacterStatComponent>(TEXT("CharacterStat"));

	//////// UI - HealthBar 추가  ////////
	HpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	HpWidget->SetupAttachment(RootComponent);
	HpWidget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	HpWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidget(TEXT("/Game/UI/WB_HealthBar.WB_HealthBar_C"));
	if (HpBarWidget.Succeeded())
	{
		HpWidget->SetWidgetClass(HpBarWidget.Class);
		HpWidget->SetDrawSize(FVector2D(150.f, 15.f));
	}
}

void AStlCppCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StlAnim = Cast<UStlAnimInstance>(GetMesh()->GetAnimInstance());
	check(StlAnim != nullptr);

	StlAnim->OnMontageEnded.AddDynamic(this, &AStlCppCharacter::OnAttackMontageEnded);

	StlAnim->OnNextAttackCheck.AddLambda([this]()->void {
	
		bCanNextCombo = false;
		if (bIsComboInput)
		{
			AttackStartCombo();
			StlAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		
	});


	HpWidget->InitWidget(); // 완기 나이스
	UHpBarWidget* MyHealthBar = Cast<UHpBarWidget>(HpWidget->GetUserWidgetObject());
	if (MyHealthBar)
	{
		MyHealthBar->BindCharcterStat(CharacterStat);
	}
}
float AStlCppCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CharacterStat) // 캐릭터스탯 컴포넌트가 존재하면
	{
		CharacterStat->SetCurrentHpByDamage(Damage);
	}
	return Damage;
}
// Called when the game starts or when spawned
void AStlCppCharacter::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AStlCppCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttackCheck)
	{
		AttackCheck();
	}
}

// Called to bind functionality to input
void AStlCppCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AStlCppCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AStlCppCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AStlCppCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AStlCppCharacter::Turn);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AStlCppCharacter::Attack);
}

void AStlCppCharacter::MoveForward(float Value)
{
	//AddMovementInput(GetActorForwardVector(), Value);

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Value);
}
void AStlCppCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector(), Value);
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}
void AStlCppCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
void AStlCppCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AStlCppCharacter::Attack()
{
	if (bIsAttacking)
	{
		if (bCanNextCombo)
		{
			bIsComboInput = true;
		}
	}
	else
	{
		AttackStartCombo();
		StlAnim->PlayAttackMontage();
		StlAnim->JumpToAttackMontageSection(CurrentCombo);
		bIsAttacking = true;
	}
}

void AStlCppCharacter::AttackStartCombo()
{
	bCanNextCombo = true;
	bIsComboInput = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}
void AStlCppCharacter::AttackEndCombo()
{
	bCanNextCombo = false;
	bIsComboInput = false;
	CurrentCombo = 0;
}

void AStlCppCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	AttackEndCombo();
}

void AStlCppCharacter::AddWeaponToCharacter(AActor* Weapon)
{
	auto MyWeapon = Cast<ABaseWeapon>(Weapon);
	if (MyWeapon)
	{
		MyWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Weapon")));
		MyWeapon->SetOwner(this);
		CurrentWeapon = MyWeapon;
	}
}

bool AStlCppCharacter::CanGetWeapon()
{
	if (!CurrentWeapon)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

void AStlCppCharacter::AttackCheck()
{
	//Todo
	UE_LOG(LogTemp, Warning, TEXT("AttackChecking"));
	//ECC_GameTraceChannel1
	TArray<FHitResult> HitArray;
	FVector Attack_Start = GetMesh()->GetSocketLocation(FName(TEXT("Attack_Start")));
	FVector Attack_End = GetMesh()->GetSocketLocation(FName(TEXT("Attack_End")));
	//GetWorld()->LineTraceMultiByChannel(HitArray, Attack_Start, Attack_End, ECollisionChannel::ECC_GameTraceChannel1);
	//DrawDebugLine(GetWorld(), Attack_Start, Attack_End, FColor::Red, false, 1.f, 0, 2.f);
	
	bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), 
		Attack_Start, 
		Attack_End, 
		25.f, 
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), 
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::ForDuration, 
		HitArray, true, FLinearColor::Red, FLinearColor::Green, 1.f);

	if (Hit)
	{
		for (const auto& HitResult : HitArray)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
			ActorsToIgnore.Add(HitResult.GetActor());
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), CharacterStat->GetCharacterAttackDamage(), UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);
		}
	}
}
