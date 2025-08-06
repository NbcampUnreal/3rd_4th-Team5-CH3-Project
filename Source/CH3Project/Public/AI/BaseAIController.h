#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class CH3PROJECT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();
	
	FVector PatrolCenter;

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const 
	{ 
		return BlackboardComp; 
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

};
