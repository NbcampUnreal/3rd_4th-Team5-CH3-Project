// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grenade/BaseGrenade.h"
#include "Grenade.generated.h"


UCLASS()
class CH3PROJECT_API AGrenade : public ABaseGrenade
{
	GENERATED_BODY()


protected:
	virtual void Explode() override;

	UPROPERTY(EditAnywhere, Category="Grenade")
	float ExplosionRadius = 300.f;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	float Damage = 50.f;
};
