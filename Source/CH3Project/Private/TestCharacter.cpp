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


//* 참고용 인벤토리 시스템 구조


void ATestCharacter::AddWeaponToInventory(AWeapon* NewWeapon)
{
	if (NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		NewWeapon->SetOwner(this);

		// 현재 장착 중인 무기인 경우 숨기지 말 것
		if (NewWeapon != CurrentWeapon)
		{
			NewWeapon->SetActorHiddenInGame(true);
			NewWeapon->SetActorEnableCollision(false);
		}

		Inventory.AddUnique(NewWeapon); // 중복 방지

		if (CurrentWeapon == nullptr)
		{
			SelectWeaponByIndex(0); // 첫 무기 장착
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Inventory Count: %d"), Inventory.Num());
		for (auto Weapon : Inventory)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon: %s"), *Weapon->GetName());
		}
		
	}
}

void ATestCharacter::RemoveWeaponFromInventory(AWeapon* WeaponToRemove)
{
	if (WeaponToRemove)
	{
		if (WeaponToRemove == CurrentWeapon)
		{
			CurrentWeapon->SetActorHiddenInGame(true);
			CurrentWeapon = nullptr;
		}
		Inventory.Remove(WeaponToRemove);
		WeaponToRemove->Destroy();
	}
}

void ATestCharacter::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip || WeaponToEquip == CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipWeapon aborted: invalid weapon or already equipped"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon: %s"), *WeaponToEquip->GetName());

	// 현재 무기 숨기기
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); 
	}
	
	CurrentWeapon = WeaponToEquip;

	// 무기 붙이기
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);

	CurrentWeapon -> SetActorRelativeRotation(FRotator(85.0f, 130.0f, 0.0f)); //애니메이션 미적용 중이기 때문에 테스트용 코드
	
	// 반드시 보여지도록 처리
	CurrentWeapon->SetActorHiddenInGame(false);
	CurrentWeapon->SetActorEnableCollision(true);

	UE_LOG(LogTemp, Warning, TEXT("Weapon equipped successfully: %s"), *CurrentWeapon->GetName());

	
}

void ATestCharacter::SelectWeaponByIndex(int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return;

	AWeapon* WeaponToEquip = Inventory[Index];
	if (WeaponToEquip)
	{
		EquipWeapon(WeaponToEquip);
		CurrentWeaponIndex = Index;
	}
}

void ATestCharacter::CycleWeaponUp()
{
	if (Inventory.Num() <= 1) return;

	int32 NewIndex = (CurrentWeaponIndex + 1) % Inventory.Num();
	SelectWeaponByIndex(NewIndex);
}

void ATestCharacter::CycleWeaponDown()
{
	if (Inventory.Num() <= 1) return;

	int32 NewIndex = (CurrentWeaponIndex - 1 + Inventory.Num()) % Inventory.Num();
	SelectWeaponByIndex(NewIndex);
}

void ATestCharacter::HandleMouseScroll(const FInputActionValue& Value)
{
	float ScrollDelta = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollDelta))
		return;

	CurrentWeaponIndex += (ScrollDelta > 0) ? 1 : -1;

	if (CurrentWeaponIndex >= Inventory.Num())
		CurrentWeaponIndex = 0;
	else if (CurrentWeaponIndex < 0)
		CurrentWeaponIndex = Inventory.Num() - 1;

	SelectWeaponByIndex(CurrentWeaponIndex);
}


//* 여기까지 참고용 코드입니다.

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
		CurrentWeapon->StartFire();
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

FVector ATestCharacter::GetAimTargetLocation() const //함수 내용은 달라도 GetAimTargetLocation 함수명은 고정, 혹은 수정 시 알려주세요


{
	return GetActorLocation() + GetActorForwardVector() * 1000.0f; // 기본적으로 정면으로 10,000 유닛
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

