// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "TestCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);


	PickupTrigger=CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
	PickupTrigger->SetupAttachment(Mesh);
	PickupTrigger->SetSphereRadius(100.f); // 픽업 트리거의 반지름 설정
	PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 활성화
	PickupTrigger->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널에 대해 충돌 무시
	PickupTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn 채널에 대해서만 오버랩 응답 설정


}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

	CurrentAmmo = MaxAmmo;
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::StartFire()
{
	if (FireMode == EFireMode::SemiAuto)
	{
		HandleFire();
	}
	else if (FireMode == EFireMode::FullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &AWeapon::HandleFire, 0.1f, true); // 0.1초 간격으로 자동 발사
	}
}

void AWeapon::StopFire()
{
	if (FireMode == EFireMode::FullAuto)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle); // 자동 발사 중지
	}
}

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

	FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
	FVector SpawnLocation = MuzzleOffset->GetComponentLocation();	

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner())) //캐릭터 명 수정 필요
	{
		if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
		{
			int32 x, y;
			PC->GetViewportSize(x, y);
			FVector WorldCenter, WorldDir;
			PC->DeprojectScreenPositionToWorld(x / 2.0f, y / 2.0f, WorldCenter, WorldDir);
			WorldCenter += WorldDir * 10000;
			SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, WorldCenter);
		}
	}

	if (BulletActor)
	{
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		GetWorld()->SpawnActor<ABullet>(BulletActor, SpawnLocation, SpawnRotation, Params);
	}
}

void AWeapon::Reload()
{
	if (bIsReloading || CurrentAmmo == MaxAmmo) return;

	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &AWeapon::FinishReload, ReloadTime, false);
}


void AWeapon::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATestCharacter* Character = Cast<ATestCharacter>(OtherActor);
	if (Character)
	{
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
		SetOwner(Character);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::SetMaxAmmo(int32 Value)
{
	MaxAmmo = Value;
	CurrentAmmo = MaxAmmo;
}

void AWeapon::SetReloadTime(float Time)
{
	ReloadTime = Time;
}

void AWeapon::SetFireMode(EFireMode Mode)
{
	FireMode = Mode;
}

void AWeapon::SetBullet(TSubclassOf<ABullet> Bullet)
{
	BulletActor = Bullet;
}
