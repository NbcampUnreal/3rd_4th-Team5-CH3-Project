// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Weapon/Weapon.h"
#include "ItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API AItemWeapon : public AItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	AWeapon* SpawnWeapon(AActor* Owner);

};
