// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "AssaultRifleBullet.generated.h"


UCLASS()
class CH3PROJECT_API AAssaultRifleBullet : public ABullet
{
	GENERATED_BODY()

public:
	AAssaultRifleBullet();

protected:
	virtual void BeginPlay() override;


	
};
