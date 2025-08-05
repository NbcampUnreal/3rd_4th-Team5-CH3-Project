// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/AssaultRifleBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"



AAssaultRifleBullet::AAssaultRifleBullet()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = 3000.0f;
		ProjectileMovement->MaxSpeed = 3000.0f;
	}

	Damage = 25.0f;
}

void AAssaultRifleBullet::BeginPlay()
{
	Super::BeginPlay();
}

