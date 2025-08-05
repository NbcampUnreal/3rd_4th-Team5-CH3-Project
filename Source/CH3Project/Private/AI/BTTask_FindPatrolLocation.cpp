#include "AI/BTTask_FindPatrolLocation.h"
<<<<<<< HEAD
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
=======
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
>>>>>>> origin/Gamemode

UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("Find Patrol Location");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
<<<<<<< HEAD
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
=======
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;
	if (AIController == nullptr || AIController->GetPawn() == nullptr)
>>>>>>> origin/Gamemode
	{
		return EBTNodeResult::Failed;
	}

<<<<<<< HEAD
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
=======
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(AIPawn->GetWorld());
>>>>>>> origin/Gamemode
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}	

	FNavLocation RandomLocation;
<<<<<<< HEAD
	if(NavSystem->GetRandomPointInNavigableRadius(AIController->PatrolCenter, SearchRadius, RandomLocation, nullptr))
=======
	if(NavSystem->GetRandomPointInNavigableRadius(AIPawn->GetActorLocation(), SearchRadius, RandomLocation, nullptr))
>>>>>>> origin/Gamemode
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
