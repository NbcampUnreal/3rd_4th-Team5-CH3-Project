#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_ClearBBValue.generated.h"


UCLASS()
class CH3PROJECT_API UBTTask_ClearBBValue : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTTask_ClearBBValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector KeyToClear;
};