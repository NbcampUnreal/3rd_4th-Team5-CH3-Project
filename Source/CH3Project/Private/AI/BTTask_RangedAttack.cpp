#include "AI/BTTask_RangedAttack.h"
#include "AI/BaseAICharacter.h"
#include "AIController.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");	
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(AIController->GetPawn());
	if (AICharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AICharacter->PerformRangedAttack();

	return EBTNodeResult::Succeeded;
}
