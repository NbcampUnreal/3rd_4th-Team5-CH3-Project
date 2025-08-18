//샷건 구현은 잠정 보류, 스폰 불릿 액터끼리 서로 겹치는 현상

#include "Weapon/Shotgun.h"
#include "Bullet.h"

AShotgun::AShotgun()
{
	SetMaxAmmo(10);
	SetReloadTime(3.0f);
	SetFireMode(EFireMode::SemiAuto);
	
	PelletCount = 8;
	SpreadAngle = 10.0f;
};

void AShotgun::HandleFire()
{
	// 탄약이 없거나 재장전 중이면 발사 불가
	if (bIsReloading || CurrentAmmo <= 0)
	{
		if (CurrentAmmo <= 0)
		{
			Reload(); // 탄약 없으면 재장전
		}
		return;
	}

	// 탄약 소모
	CurrentAmmo--;


	FVector SpawnLocation = MuzzleOffset->GetComponentLocation();
	FRotator BaseRotation = MuzzleOffset->GetComponentRotation();

	

	
	FActorSpawnParameters Params;
	Params.Owner = GetOwner(); // 총의 소유자 지정 (중요: 총알에 의한 데미지 소스 추적 가능)
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 부채꼴로 총알 여러 개 생성
	for (int32 i = 0; i < PelletCount; ++i)
	{
		// 퍼짐 각도를 Yaw/Pitch에 랜덤하게 적용
		float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
		float RandomPitch = FMath::RandRange(-SpreadAngle / 2.0f, SpreadAngle / 2.0f);

		FRotator SpreadRotation = BaseRotation;
		SpreadRotation.Yaw += RandomYaw;
		SpreadRotation.Pitch += RandomPitch;

		// 총알 액터 생성
		if (BulletActor)
		{
			GetWorld()->SpawnActor<ABullet>(BulletActor, SpawnLocation, SpreadRotation, Params);
		}
	}

	// 사운드 및 이펙트 재생
	PlayFireEffects();
}

