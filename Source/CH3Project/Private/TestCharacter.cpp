// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"



ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AutoEquipWeapon();
}

void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//오버랩 시 플레이어 캐릭터의 손 소켓에 무기 부착
void ATestCharacter::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip || CurrentWeapon) return;

	CurrentWeapon = WeaponToEquip;

	if (GetMesh())
	{
		WeaponToEquip->AttachToComponent(
			GetMesh(),
			
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("RightHandSocket")  // 에디터에서 지정한 소켓 이름, 스켈레탈 메쉬마다 반드시 추가 필요
		);

		WeaponToEquip->SetActorRelativeRotation(FRotator(80.f, 150.f, 30.f)); //애니메이션 미 적용 상태 반영본으로 추후 수정, 혹은 삭제 가능
		WeaponToEquip->SetActorRelativeScale3D(FVector(0.5f)); // 습득한 무기의 크기를 조절하는 파트
		
		WeaponToEquip->SetOwner(this);
	}
}


void ATestCharacter::FireWeapon()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void ATestCharacter::StopFireWeapon()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}




//이걸 추후에 캐릭터에 작성해주세요
FVector ATestCharacter::GetAimTargetLocation() const
{
	return GetActorLocation() + GetActorForwardVector() * 10000.0f; // 기본적으로 정면으로 10,000 유닛
} //탑뷰 형식으로 구현할 시 반드시 수정 필요, 캐릭터가 바라보는 정면으로 사격됨

void ATestCharacter::OnReload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

//스폰 시 자동 장착할 무기 선택 함수 AI, 캐릭터
void ATestCharacter::AutoEquipWeapon()
{
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(
			WeaponClass,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParams
		);

		if (SpawnedWeapon)
		{
			EquipWeapon(SpawnedWeapon);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Weapon spawn failed"));
		}
	}
}