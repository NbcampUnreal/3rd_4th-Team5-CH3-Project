#include "AI/BTTask_MeleeAttack.h"
#include "AI/BaseAICharacter.h"
#include "AIController.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if(AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(AIController->GetPawn());
	if(AICharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AICharacter->PerformMeleeAttack();

	return EBTNodeResult::Succeeded;
}
