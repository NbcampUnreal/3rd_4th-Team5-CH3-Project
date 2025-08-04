#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_MeleeAttack.generated.h"


UCLASS()
class CH3PROJECT_API UBTTask_MeleeAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTTask_MeleeAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
