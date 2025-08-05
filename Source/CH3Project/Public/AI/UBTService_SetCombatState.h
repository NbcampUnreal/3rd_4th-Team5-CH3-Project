#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "UBTService_SetCombatState.generated.h"


UCLASS()
class CH3PROJECT_API UUBTService_SetCombatState : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UUBTService_SetCombatState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MeleeRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RangedRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector CombatStateKey;

};
