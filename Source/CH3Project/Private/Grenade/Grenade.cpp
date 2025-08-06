// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade/Grenade.h"
#include "Kismet/GameplayStatics.h"



void AGrenade::Explode()
{
	Super::Explode();

	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		GetActorLocation(),
		ExplosionRadius,
		nullptr,
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true
		);
}

