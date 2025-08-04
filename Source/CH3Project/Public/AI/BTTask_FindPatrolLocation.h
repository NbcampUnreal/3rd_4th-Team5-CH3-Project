#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_FindPatrolLocation.generated.h"


UCLASS()
class CH3PROJECT_API UBTTask_FindPatrolLocation : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SearchRadius = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector PatrolLocationKey;
};
