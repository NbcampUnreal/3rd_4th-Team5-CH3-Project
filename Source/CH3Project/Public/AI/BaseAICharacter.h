#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

class AAIWeaponActor;
class UNiagaraSystem;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMeshComp;

	void setMoveSpeed(float NewSpeed);
	void PerformMeleeAttack();
	void PerformRangedAttack();
	void HandleDeath();



protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	UNiagaraSystem* MuzzleEffect;
	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	UNiagaraSystem* HitEffect;

	class UAIHealthComponent* HealthComponent;

};