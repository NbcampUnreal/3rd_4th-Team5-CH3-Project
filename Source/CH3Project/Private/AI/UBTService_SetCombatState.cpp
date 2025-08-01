#include "AI/UBTService_SetCombatState.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BaseAICharacter.h"

UUBTService_SetCombatState::UUBTService_SetCombatState()
{

	NodeName = TEXT("Set Combat State");
	Interval = 1.0f;

	MeleeRange = 100.0f;
	RangedRange = 1000.0f;
}

void UUBTService_SetCombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(AIController == nullptr || BlackboardComp == nullptr)
	{
		return;
	}

	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(AIController->GetPawn());
	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);
	
	if (AICharacter == nullptr || TargetActor == nullptr)
	{
		BlackboardComp->SetValueAsEnum(CombatStateKey.SelectedKeyName, 0); // 0 for Idle state
		return;
	}

	float DistanceToTarget = FVector::Dist(AICharacter->GetActorLocation(), TargetActor->GetActorLocation());

	if (DistanceToTarget <= MeleeRange)
	{
		BlackboardComp->SetValueAsEnum(CombatStateKey.SelectedKeyName, 1); // 1 for Melee state
	}
	else if (DistanceToTarget <= RangedRange)
	{
		BlackboardComp->SetValueAsEnum(CombatStateKey.SelectedKeyName, 2); // 2 for Ranged state
	}
	else
	{
		BlackboardComp->SetValueAsEnum(CombatStateKey.SelectedKeyName, 0); // 0 for Idle state
	}
}
