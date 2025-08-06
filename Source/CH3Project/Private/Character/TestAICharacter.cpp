// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestAICharacter.h"


ATestAICharacter::ATestAICharacter()
{
	if (WeaponClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AWeapon> WeaponBP(TEXT("/Script/Engine.Blueprint'/Game/Weapon/AssultRifle/Weapon/BP_AssaulRifle.BP_AssaulRifle'"));
		if (WeaponBP.Succeeded())
		{
			WeaponClass = WeaponBP.Class;
		}
	}
}

	void ATestAICharacter::BeginPlay()
	{
		Super::BeginPlay();
		// 무기 스폰은 부모가 처리하므로 추가 코드 불필요
	}