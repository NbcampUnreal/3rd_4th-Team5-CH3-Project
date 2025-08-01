#include "AI/BTTask_SetBBvalueAsBool.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBBvalueAsBool::UBTTask_SetBBvalueAsBool()
{
	NodeName = TEXT("Set Blackboard Value As Bool");
}

EBTNodeResult::Type UBTTask_SetBBvalueAsBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	BlackboardComp->SetValueAsBool(BoolKey.SelectedKeyName, BoolValue);
	return EBTNodeResult::Succeeded;
}
