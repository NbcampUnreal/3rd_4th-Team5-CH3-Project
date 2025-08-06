// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/CH3PlayerController.h"
#include "Weapon/Weapon.h"


ACH3Character::ACH3Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	DashSpeed = 2000.0f; // DashSpeed 초기화
}

void ACH3Character::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentWeaponInstance = GetWorld()->SpawnActor<AWeapon>(CurrentWeapon); // CurrentWeapon 클래스를 사용하여 무기 인스턴스를 생성
	GetMesh()->HideBoneByName(TEXT("weapon_r"),EPhysBodyOp::PBO_None);
	CurrentWeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket")); // 무기를 캐릭터의 손에 붙임.
	CurrentWeaponInstance->SetOwner(this); // 무기의 소유자를 현재 캐릭터로 설정
}

void ACH3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ACH3PlayerController* MyController = Cast<ACH3PlayerController>(GetController()))
		{
			if (MyController->MoveAction)
			{
				EnhancedInputComponent->BindAction(MyController->MoveAction, ETriggerEvent::Triggered, this, &ACH3Character::Move);
			}
			if (MyController->EquipAction)
			{
				EnhancedInputComponent->BindAction(MyController->EquipAction, ETriggerEvent::Triggered, this, &ACH3Character::Equip);
			}
			if (MyController->FireAction)
			{
				EnhancedInputComponent->BindAction(MyController->FireAction, ETriggerEvent::Triggered, this, &ACH3Character::Fire);
			}
			if (MyController->DashAction)
			{
				EnhancedInputComponent->BindAction(MyController->DashAction, ETriggerEvent::Triggered, this, &ACH3Character::Dash);
			}
		}
	}

}

void ACH3Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 탑다운뷰 에서의 이동 방향을 카메라의 방향에 맞춰 조정
	AddMovementInput(CameraComp->GetForwardVector(), MovementVector.X);
	AddMovementInput(CameraComp->GetRightVector(), MovementVector.Y);
}

void ACH3Character::Equip(const FInputActionValue& Value)
{
	EquipWeapon(CurrentWeapon); // 현재 장착된 무기를 장착하는 함수 호출

	UE_LOG(LogTemp, Warning, TEXT("Equip action triggered!"));
}

void ACH3Character::Fire(const FInputActionValue& Value)
{
	// 총 발사 로직을 여기에 구현예정
	UE_LOG(LogTemp, Warning, TEXT("Fire action triggered!"));
}

void ACH3Character::Dash(const FInputActionValue& Value)
{

	FVector DashDirection = GetMesh()->GetRightVector().GetSafeNormal(); // 대시 방향 설정
	FVector DashVelocity = DashDirection * DashSpeed; // 대시 속도 설정

	LaunchCharacter(DashVelocity, true, true); // 캐릭터를 대시 방향으로 발사

	// 대시 쿨타임 구현예정
}

void ACH3Character::EquipWeapon(TSubclassOf<AWeapon> NewWeaponClass) // 무기를 장착하는 함수
{
	if (NewWeaponClass)
	{
		if (CurrentWeaponInstance)
		{
			CurrentWeaponInstance->Destroy(); // 현재 장착된 무기가 있다면 파괴
		}
		CurrentWeaponInstance = GetWorld()->SpawnActor<AWeapon>(NewWeaponClass); // 새로운 무기 인스턴스를 생성
		if (CurrentWeaponInstance)
		{
			CurrentWeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket")); // 무기를 캐릭터의 손에 붙임
			CurrentWeaponInstance->SetOwner(this); // 무기의 소유자를 현재 캐릭터로 설정
			GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None); // 무기 뼈대를 숨김
		}
	}
}