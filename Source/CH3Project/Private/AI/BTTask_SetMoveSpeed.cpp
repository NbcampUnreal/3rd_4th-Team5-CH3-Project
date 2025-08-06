#include "AI/BTTask_SetMoveSpeed.h"
#include "AI/BaseAICharacter.h"
#include "AIController.h"

UBTTask_SetMoveSpeed::UBTTask_SetMoveSpeed()
{
	NodeName = TEXT("Set Move Speed");
}

EBTNodeResult::Type UBTTask_SetMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		ABaseAICharacter* AIChar = Cast<ABaseAICharacter>(AIController->GetPawn());
		if (AIChar)
		{
			AIChar->setMoveSpeed(NewSpeed);
			return EBTNodeResult::Succeeded;	
		}
	}
	return EBTNodeResult::Failed;
}
