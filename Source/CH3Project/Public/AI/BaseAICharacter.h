#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

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

	void setMoveSpeed(float NewSpeed);
	void PerformMeleeAttack();
	void PerformRangedAttack();

protected:
	virtual void BeginPlay() override;

	class UAIHealthComponent* HealthComponent;

};