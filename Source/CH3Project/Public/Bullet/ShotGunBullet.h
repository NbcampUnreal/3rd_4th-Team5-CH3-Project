// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "ShotGunBullet.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API AShotGunBullet : public ABullet
{
	GENERATED_BODY()


public:
	AShotGunBullet();


protected:
	virtual void BeginPlay() override;
};
