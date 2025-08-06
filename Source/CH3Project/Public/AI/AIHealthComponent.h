#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3PROJECT_API UAIHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UAIHealthComponent();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.0f;
<<<<<<< HEAD
=======

>>>>>>> origin/Gamemode
	float CurrentHealth;
};
