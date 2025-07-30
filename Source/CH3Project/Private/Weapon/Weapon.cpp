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
	PickupTrigger->SetSphereRadius(100.f); // �Ⱦ� Ʈ������ ������ ����
	PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // �浹 Ȱ��ȭ
	PickupTrigger->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� ä�ο� ���� �浹 ����
	PickupTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn ä�ο� ���ؼ��� ������ ���� ����


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
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &AWeapon::HandleFire, 0.1f, true); // 0.1�� �������� �ڵ� �߻�
	}
}

void AWeapon::StopFire()
{
	if (FireMode == EFireMode::FullAuto)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle); // �ڵ� �߻� ����
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

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner())) //ĳ���� �� ���� �ʿ�
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
