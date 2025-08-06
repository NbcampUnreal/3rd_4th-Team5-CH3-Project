#include "AI/AIWeaponActor.h"
#include "Components/StaticMeshComponent.h"


AAIWeaponActor::AAIWeaponActor()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

