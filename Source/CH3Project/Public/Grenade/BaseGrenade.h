// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGrenade.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class CH3PROJECT_API ABaseGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseGrenade();

protected:
	virtual void BeginPlay() override;
	virtual void Explode();

	void StartFuseTimer();

	//오버랩 시 습득하는 구조, 미구현 코드
	/*
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	*/

	TSubclassOf<AActor> GetGrenadeProjectileClass() const { return GrenadeProjectileClass; }

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GrenadeMesh;
	
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* PickupTrigger; //수류탄 클래스 계열의 픽업 오버랩

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	float FuseTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	USoundBase* ExplosionSound;

	FTimerHandle FuseTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<AActor> GrenadeProjectileClass;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Throw(FVector Direction, float Force);

};
