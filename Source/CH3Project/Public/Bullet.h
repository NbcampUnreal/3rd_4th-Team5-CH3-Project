// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class CH3PROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;
	
public:	
	virtual void Tick(float DeltaTime) override;

	//불릿이 Pawn을 상속받는 캐릭터에 오버랩 시 동작하도록 설정
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	//Bullet 액터의 거리 초과 이후 메모리 삭제를 위한 탄환의 최대 사정거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	float MaxDistance = 10000.0f; //사정거리 수정 시 이 함수를 생성자에 불러와서 수정할 것
	
	FVector SpawnLocation;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	float DefaultDamage = 10.0f;

	
	
};
