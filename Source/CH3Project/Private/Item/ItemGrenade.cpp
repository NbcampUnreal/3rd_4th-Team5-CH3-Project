// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemGrenade.h"



ABaseGrenade* AItemGrenade::SpawnAntThrowGrenade(AActor* InOwner, FVector Direction, float Force)
{
	if (GrenadeClass || !InOwner) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InOwner;
	SpawnParams.Instigator = InOwner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = InOwner->GetActorLocation() + Direction * 100.0f + FVector(0, 0, 50.0f);
	FRotator SpawnRotation = Direction.Rotation();

	ABaseGrenade* Grenade = InOwner->GetWorld()->SpawnActor<ABaseGrenade>(
	GrenadeClass,
	SpawnLocation,
	SpawnRotation,
	SpawnParams
	);

	if (Grenade)
	{
		Grenade -> Throw(Direction, Force);
	}

	return Grenade;
	
}
