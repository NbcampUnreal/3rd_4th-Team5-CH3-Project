// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestAICharacter.h"



void ATestAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(
			WeaponClass,
			GetActorLocation(), //캐릭터 근처에 스폰
			GetActorRotation(), //회전은 캐릭터 기준
			SpawnParams
		);

		if (SpawnedWeapon)
		{
			EquipWeapon(SpawnedWeapon);  // 부모 클래스 함수 그대로 사용
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Weapon spawn failed"));
		}
	}
}