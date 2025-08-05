#include "AI/BaseAIController.h"
<<<<<<< HEAD
#include "AI/BaseAICharacter.h"	
=======
>>>>>>> origin/Gamemode
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

ABaseAIController::ABaseAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1000.0f;
		SightConfig->LoseSightRadius = 1500.0f;
		SightConfig->PeripheralVisionAngleDegrees = 120.0f;
		SightConfig->SetMaxAge(1.0f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerception->ConfigureSense(*SightConfig);
		AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdate);
	}
}


void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller possessed pawn: %s"), *InPawn->GetName());
	}

<<<<<<< HEAD
	ABaseAICharacter* AIChar = Cast<ABaseAICharacter>(InPawn);
	if (AIChar)
	{
		PatrolCenter = AIChar->PatrolCenter;
	}
	
=======
>>>>>>> origin/Gamemode
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
<<<<<<< HEAD

=======
>>>>>>> origin/Gamemode
}

void ABaseAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (!Actor->ActorHasTag("Enemy") || GetBlackboardComponent() == nullptr)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		DrawDebugString(GetWorld(), Actor->GetActorLocation() + FVector(0, 0, 100), FString::Printf(TEXT("Detected: %s"), *Actor->GetName()), nullptr, FColor::Green, 2.0f, true);

		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
		GetBlackboardComponent()->ClearValue(TEXT("LastKnownLocation"));
	}
	
	else
	{
		DrawDebugString(GetWorld(), Actor->GetActorLocation() + FVector(0, 0, 100), FString::Printf(TEXT("Lost: %s"), *Actor->GetName()), nullptr, FColor::Red, 2.0f, true);
		bool bIsCurrentlySearching = GetBlackboardComponent()->GetValueAsBool(TEXT("IsSearching"));

		if (!bIsCurrentlySearching)
		{
			GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownLocation"), Stimulus.StimulusLocation);
			GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		}
	}
}
