// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"



ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 오버랩만 감지
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 캐릭터에만 반응

	StaticMesh->SetGenerateOverlapEvents(true);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 20000.0f;
	ProjectileMovement->MaxSpeed = 20000.0f;

	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	Damage = DefaultDamage;
	
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
}


void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComponent)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
	} //TakeDamage(); 함수를 캐릭터 파트에 구현 필요

	Destroy();
}