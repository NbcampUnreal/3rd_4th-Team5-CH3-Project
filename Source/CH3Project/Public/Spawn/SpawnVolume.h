// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnVolume.generated.h"

UCLASS()
class CH3PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();
protected:
	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TObjectPtr<UBoxComponent> SpawnBox;

public:
	// 영역 내부 임의 위치 (네비게이션 고려 X)
	UFUNCTION(BlueprintCallable, Category="Spawn")
	FVector GetRandomPointInVolume();
	



};
