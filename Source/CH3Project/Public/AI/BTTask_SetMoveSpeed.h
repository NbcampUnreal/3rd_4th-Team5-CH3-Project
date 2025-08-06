#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMoveSpeed.generated.h"


UCLASS()
class CH3PROJECT_API UBTTask_SetMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetMoveSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float NewSpeed;
	
};
