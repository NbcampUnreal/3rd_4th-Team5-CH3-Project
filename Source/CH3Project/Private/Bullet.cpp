// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TestCharacter.h" // ĳ���� ���ϸ����� ���� �ʿ�

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

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) //�浹 �� ����Ǵ� �Լ�
{
	ATestCharacter* HitCharacter = Cast<ATestCharacter>(OtherActor); // ĳ���� ���ϸ����� ���� �ʿ�
	AActor* Actor = GetOwner();
	if (!IsValid(Actor))
	{
		return;
	}
	ATestCharacter* BulletHit = Cast<ATestCharacter>(Actor->GetOwner()); // ĳ���� ���ϸ����� ���� �ʿ�		
	if (!IsValid(BulletHit))
	{
		return;

	}
	//if (HitCharacter && BulletHit)
	//{
		//->HitCharacter(BulletHit->Strength, BulletHit); // ���� ĳ���� ������ ���� �ʿ�

		//Destroy();
	//}
}