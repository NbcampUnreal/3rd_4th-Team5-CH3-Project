#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_SetBBvalueAsBool.generated.h"


UCLASS()
class CH3PROJECT_API UBTTask_SetBBvalueAsBool : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTTask_SetBBvalueAsBool();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector KeyToSet;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bValueSet;
};
