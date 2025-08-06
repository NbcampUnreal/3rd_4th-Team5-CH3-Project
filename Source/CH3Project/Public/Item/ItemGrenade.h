// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Grenade/BaseGrenade.h"
#include "ItemGrenade.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API AItemGrenade : public AItemBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<ABaseGrenade> GrenadeClass;

	ABaseGrenade* SpawnAntThrowGrenade(AActor* InOwner, FVector Direction, float Force);
	
};
