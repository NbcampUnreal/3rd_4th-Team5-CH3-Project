#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIWeaponActor.generated.h"

UCLASS()
class CH3PROJECT_API AAIWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAIWeaponActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")	
	UStaticMeshComponent* WeaponMesh;

};
