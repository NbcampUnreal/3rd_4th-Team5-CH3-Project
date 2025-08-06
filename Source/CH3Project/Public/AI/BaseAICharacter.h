#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

class AAIWeaponActor;	

UCLASS()
class CH3PROJECT_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 600.0f;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category = "AI")
	FVector PatrolCenter;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AAIWeaponActor> WeaponClass;
	
	UPROPERTY()
	AAIWeaponActor* CurrentWeapon;


	void setMoveSpeed(float NewSpeed);
	void PerformMeleeAttack();
	void PerformRangedAttack();

	void HandleDeath();


protected:
	virtual void BeginPlay() override;

	class UAIHealthComponent* HealthComponent;

};