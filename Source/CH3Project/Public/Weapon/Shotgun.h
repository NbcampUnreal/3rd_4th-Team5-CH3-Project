// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API AShotgun : public AWeapon
{
	GENERATED_BODY()

public:
	AShotgun();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shotgun")
	int32 PelletCount = 8; // 발사되는 총알 수

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shotgun")
	float SpreadAngle = 10.0f; // 부채꼴 각도 (도 단위)

	virtual void HandleFire() override;
};
