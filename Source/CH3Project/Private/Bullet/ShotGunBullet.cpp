// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/ShotGunBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

AShotGunBullet::AShotGunBullet()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = 2000.0f;
		ProjectileMovement->MaxSpeed = 2000.0f;
	}

	Damage = 10.0f;
}

void AShotGunBullet::BeginPlay()
{
	Super::BeginPlay();
}