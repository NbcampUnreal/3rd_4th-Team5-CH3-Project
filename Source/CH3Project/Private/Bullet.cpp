// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	// 콜리전 설정: WorldDynamic, 오버랩 이벤트는 받되, 불릿끼리는 충돌 및 오버랩 무시
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// 모든 채널에 대해 오버랩 응답을 주되, 불릿끼리는 오버랩 무시를 위해 별도 처리할 것
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

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
	StaticMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	SpawnLocation = GetActorLocation();

	if (TrailEffect)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailEffect,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	// 자기 자신 소유자 무시
	if (AActor* OwnerActor = GetOwner())
	{
		StaticMesh->IgnoreActorWhenMoving(OwnerActor, true);
	}

	// 모든 다른 Bullet 및 자식 클래스 무시
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABullet::StaticClass(), IgnoreActors);

	for (AActor* Actor : IgnoreActors)
	{
		if (Actor != this)
		{
			StaticMesh->IgnoreActorWhenMoving(Actor, true);
			StaticMesh->MoveIgnoreActors.Add(Actor);
		}
	}
}


void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float DistanceTravelled = FVector::Dist(GetActorLocation(), SpawnLocation);
	if (DistanceTravelled >= MaxDistance)
	{
		Destroy(); // 사정거리 초과 시 제거
	}
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || !OtherComp)
		return;

	// 불릿끼리 오버랩 무시
	if (OtherActor->IsA(ABullet::StaticClass()))
		return;

	// 캐릭터에 데미지 적용
	if (OtherActor->IsA(APawn::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
	}

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	Destroy();
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || !OtherComp)
		return;

	// 불릿끼리 충돌 무시
	if (OtherActor->IsA(ABullet::StaticClass()))
		return;

	// 캐릭터에 데미지 적용
	if (OtherActor->IsA(APawn::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
	}

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}

	Destroy();
}