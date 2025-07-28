// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class CH3PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
