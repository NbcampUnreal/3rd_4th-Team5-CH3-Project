#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_RangedAttack.generated.h"


UCLASS()
class CH3PROJECT_API UBTTask_RangedAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTTask_RangedAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
