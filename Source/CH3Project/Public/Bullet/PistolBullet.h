// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "PistolBullet.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API APistolBullet : public ABullet
{
	GENERATED_BODY()
	

public:
	APistolBullet();

protected:
	virtual void BeginPlay() override;


	
};
