

#include "Grenade/BaseGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"




ABaseGrenade::ABaseGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_PhysicsBody);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComp->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComp;

	// 시각용 스켈레탈 메쉬 (콜리전은 SphereComp가 담당)
	GrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetupAttachment(RootComponent);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 콜리전 끔

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 1.0f;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 0.4f;
	MovementComponent->Friction = 0.2f;
	MovementComponent->BounceVelocityStopSimulatingThreshold = 10.f;
	MovementComponent->bSweepCollision = true;
	MovementComponent->SetUpdatedComponent(CollisionComp);

}

void ABaseGrenade::BeginPlay()
{
	Super::BeginPlay();

	StartFuseTimer();
	
}

void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseGrenade::StartFuseTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FuseTimerHandle, this, &ABaseGrenade::Explode, FuseTime, false);
}


void ABaseGrenade::Explode()
{
	if (ExplosionEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	if (ExplosionSound)
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	Destroy();
}

void ABaseGrenade::Throw(FVector Direction, float Force)
{
	if (MovementComponent)
	{
		// 던질 방향으로 속도 설정 (노멀라이즈 보장)
		MovementComponent->Velocity = Direction.GetSafeNormal() * Force;
	}
}