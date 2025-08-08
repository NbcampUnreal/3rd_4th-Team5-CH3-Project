

#include "Grenade/BaseGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ABaseGrenade::ABaseGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>("GrenadeMesh");
	RootComponent = GrenadeMesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent -> bRotationFollowsVelocity = true;
	MovementComponent -> ProjectileGravityScale = 1.0f;
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