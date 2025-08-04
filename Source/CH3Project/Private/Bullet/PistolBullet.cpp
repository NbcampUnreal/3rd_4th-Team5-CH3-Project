// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/PistolBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

APistolBullet::APistolBullet()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = 1500.0f;
		ProjectileMovement->MaxSpeed = 1500.0f;
	}

	Damage = 15.0f;
}

void APistolBullet::BeginPlay()
{
	Super::BeginPlay();
}