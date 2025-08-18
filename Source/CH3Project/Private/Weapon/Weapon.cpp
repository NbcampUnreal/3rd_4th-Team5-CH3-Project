// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Character/CH3Character.h" 
#include "GameMode/CH3GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = Mesh;

	
	//컨버트 과정에서 마테리얼 미적용으로 인한 추가 보충
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatObj(TEXT("/Script/Engine.Material'/Game/Weapons_Free/Materials/M_Material.M_Material'"));
	if (MatObj.Succeeded())
	{
		Mesh->SetMaterial(0, MatObj.Object); // 0번 슬롯
	}
	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);


	PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger")); // 무기가 다른 액터와 겹칠 때 사용할 트리거 컴포넌트를 생성합니다.
	PickupTrigger->SetupAttachment(Mesh);
	PickupTrigger->SetSphereRadius(100.f);
	PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Mesh->SetRelativeRotation(FRotator(20.0f, 220.0f, 0.0f));
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); // 무기의 메쉬는 충돌을 하지 않도록 설정합니다.

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin); // 무기가 다른 액터와 겹칠 때 OnOverlapBegin 함수를 호출합니다.

	CurrentAmmo = MaxAmmo; // 현재 탄약량을 최대치로 초기화합니다.
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// StartFire 함수는 무기가 발사될 때 호출되어 발사를 시작합니다.
void AWeapon::StartFire()
{
	UE_LOG(LogTemp, Warning, TEXT("FireWeapon called"));

	if (FireMode == EFireMode::SemiAuto)
	{
		HandleFire();
	}
	else if (FireMode == EFireMode::FullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &AWeapon::HandleFire, 0.1f, true);  // 0.1초 간격으로 HandleFire 함수를 호출하여 연속 발사를 구현합니다.
	}
}

// StopFire 함수는 무기가 발사 중일 때 호출되어 발사를 중지합니다.
void AWeapon::StopFire()
{
	UE_LOG(LogTemp, Warning, TEXT("StopFireWeapon called"));

	if (FireMode == EFireMode::FullAuto)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle); 
	}
}

// HandleFire 함수는 무기가 발사될 때 호출됩니다.
void AWeapon::HandleFire()
{
	if (bIsReloading || CurrentAmmo <= 0)
	{

		if (CurrentAmmo <= 0)
		{
			Reload();
		}
		return;
	}

	CurrentAmmo--;
	auto* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		auto* MyGameMode = Cast<ACH3GameMode>(GameMode);
		if (MyGameMode)
		{
			MyGameMode->UpdateWeaponMagazine_Size(FString::FromInt(CurrentAmmo));
		}
	}
	
	FVector SpawnLocation = MuzzleOffset->GetComponentLocation();
	FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();


	// 베이스 캐릭터를 바탕으로, 플레이어 캐릭터, AI 캐릭터에 적용할 수 있는 확장성 확보

	if (ACH3Character* OwnerCharacter = Cast<ACH3Character>(GetOwner()))
	{
		FVector AimTarget = OwnerCharacter->GetAimTargetLocation(); //GetAimTargetLocation 함수를 캐릭터 쪽에서 구현해줘야 함. 현재 미구동. // 성빈 : 확인했음

		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, AimTarget);
	}
	
	if (BulletActor)
	{
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletActor, SpawnLocation, SpawnRotation, Params);

		PlayFireEffects();
	}	
}

// 재장전 상태를 확인하고, 현재 탄약이 최대치가 아니면 재장전을 시작합니다.
void AWeapon::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("리로드 중"));

	if (bIsReloading || CurrentAmmo == MaxAmmo)
	{
		return;
	}
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &AWeapon::FinishReload, ReloadTime, false);
	
}

// 재장전이 완료되면 현재 탄약을 최대치로 설정하고 재장전 상태를 해제합니다.
void AWeapon::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("리로드 완료됨. 현재 탄약: %d"), CurrentAmmo);

	auto* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		auto* MyGameMode = Cast<ACH3GameMode>(GameMode);
		if (MyGameMode)
		{
			MyGameMode->UpdateWeaponMagazine_Size(FString::FromInt(CurrentAmmo));
		}
	}
}

//오버랩 시 캐릭터의 AddWeaponToInventory 함수에 따라 인벤토리 내 저장이 됨.
void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACH3Character* Character = Cast<ACH3Character>(OtherActor); // ACH3Character로 수정

	// Stackoverflow가 발생하지 않도록 캐릭터가 무기를 획득할 때만 실행되게 수정
	if (Character) // 캐릭터가 무기를 획득할 때
	{

		if (PickupTrigger)
		{
			PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);// 오버랩이 발생하면 트리거의 충돌을 비활성화
		}


		//Character->EquipWeapon(GetClass()); // 캐릭터의 EquipWeapon 함수를 호출하도록 수정


		//this->Destroy(); // 무기를 파괴합니다.
	}
}

// 최대 탄약량을 설정하고 현재 탄약량을 최대치로 초기화합니다.
void AWeapon::SetMaxAmmo(int32 Value)
{
	MaxAmmo = Value;
	CurrentAmmo = MaxAmmo;
}

// 재장전 시간을 설정합니다.
void AWeapon::SetReloadTime(float Time)
{
	ReloadTime = Time;
}

// 무기의 발사 모드를 설정합니다.
void AWeapon::SetFireMode(EFireMode Mode)
{
	FireMode = Mode;
}

//불릿 액터를 설정합니다.
void AWeapon::SetBullet(TSubclassOf<ABullet> Bullet)
{
	BulletActor = Bullet;
}

void AWeapon::PlayFireEffects()
{
	//  사운드 재생
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSound,
			MuzzleOffset->GetComponentLocation(),
			1.0f,
			1.0f,
			0.0f,
			FireSoundAttenuation
		);
	}

	//  총구 플래시 이펙트 재생
	if (MuzzleFlashEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MuzzleFlashEffect,
			MuzzleOffset->GetComponentLocation(),
			MuzzleOffset->GetComponentRotation()
		);
	}
}

EFireMode AWeapon::GetFireMode() const // 사격 모드를 반환하는 함수
{
	return FireMode;
}

// WeaponMesh를 반환하는 GetMesh() 함수 정의
USkeletalMeshComponent* AWeapon::GetMesh() const
{
	return Mesh;
}