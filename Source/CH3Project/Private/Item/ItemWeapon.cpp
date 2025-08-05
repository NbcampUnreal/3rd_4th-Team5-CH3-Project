// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemWeapon.h"
#include "Engine/World.h"


AWeapon* AItemWeapon::SpawnWeapon(AActor* InOwner)
{
	if (!WeaponClass) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InOwner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(
		WeaponClass,
		InOwner->GetActorLocation(),
		InOwner->GetActorRotation(),
		SpawnParams
	);

	return SpawnedWeapon;
}
