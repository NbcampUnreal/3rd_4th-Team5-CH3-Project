// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TestCharacter.h" // 캐릭터 파일명으로 변경 필요

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 20000.0f;
	ProjectileMovement->MaxSpeed = 20000.0f;

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	//StaticMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) //충돌 시 실행되는 함수
{
	ATestCharacter* HitCharacter = Cast<ATestCharacter>(OtherActor); // 캐릭터 파일명으로 변경 필요
	AActor* Actor = GetOwner();
	if (!IsValid(Actor))
	{
		return;
	}
	ATestCharacter* BulletHit = Cast<ATestCharacter>(Actor->GetOwner()); // 캐릭터 파일명으로 변경 필요		
	if (!IsValid(BulletHit))
	{
		return;

	}
	//if (HitCharacter && BulletHit)
	//{
		//->HitCharacter(BulletHit->Strength, BulletHit); // 추후 캐릭터 변수로 수정 필요

		//Destroy();
	//}
}