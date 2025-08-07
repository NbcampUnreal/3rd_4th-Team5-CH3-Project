// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemGrenade.h"



ABaseGrenade* AItemGrenade::SpawnAntThrowGrenade(AActor* InOwner, FVector Direction, float Force)
{
	if (!GrenadeClass || !InOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrenadeClass is null or InOwner is invalid."));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InOwner;
	SpawnParams.Instigator = InOwner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 방향 벡터 정규화 (0 벡터 방지)
	FVector NormalizedDirection = Direction.GetSafeNormal();

	// 수류탄이 소환될 위치 계산: 플레이어 앞쪽 + 위쪽
	FVector SpawnLocation = InOwner->GetActorLocation() + NormalizedDirection * 100.0f + FVector(0, 0, 50.0f);
	FRotator SpawnRotation = NormalizedDirection.Rotation();

	ABaseGrenade* Grenade = InOwner->GetWorld()->SpawnActor<ABaseGrenade>(
		GrenadeClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (Grenade)
	{
		Grenade->Throw(NormalizedDirection, Force);
		UE_LOG(LogTemp, Log, TEXT("Grenade spawned and thrown."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn grenade."));
	}

	return Grenade;
}
