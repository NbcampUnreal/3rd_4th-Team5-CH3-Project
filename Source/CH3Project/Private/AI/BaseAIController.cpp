#include "AI/BaseAIController.h"
#include "AI/BaseAICharacter.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"

ABaseAIController::ABaseAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerception->ConfigureSense(*SightConfig);
		AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdate);
	}

	GetWorldTimerManager().SetTimer(RandomMoveTimer, this, &ABaseAIController::MoveToRandomLocation, 5.0f, true, 1.0f);
}


void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller possessed pawn: %s"), *InPawn->GetName());
	}
}


void ABaseAIController::MoveToRandomLocation()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return;
	}

	FNavLocation RandomLocation;

	bool bFoundLocation = NavSystem->GetRandomPointInNavigableRadius(MyPawn->GetActorLocation(), MoveRadius, RandomLocation);

	if (bFoundLocation)
	{
		MoveToLocation(RandomLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("AI Controller moving to random location: %s"), *RandomLocation.Location.ToString());
	}

}

void ABaseAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (Actor != PlayerPawn)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		DrawDebugString(GetWorld(), Actor->GetActorLocation() + FVector(0, 0, 100), FString::Printf(TEXT("Detected: %s"), *Actor->GetName()), nullptr, FColor::Green, 2.0f, true);

		StartChasingTarget(Actor);
	}
	else
	{
		DrawDebugString(GetWorld(), Actor->GetActorLocation() + FVector(0, 0, 100), FString::Printf(TEXT("Lost: %s"), *Actor->GetName()), nullptr, FColor::Red, 2.0f, true);
		
		StopChasingTarget();
	}
}

void ABaseAIController::StartChasingTarget(AActor* Target)
{
	if(bIsChasing && TargetActor == Target)
	{
		return;
	}

	TargetActor = Target;
	bIsChasing = true;

	GetWorldTimerManager().ClearTimer(RandomMoveTimer);

	if(ABaseAICharacter* AIChar = Cast<ABaseAICharacter>(GetPawn()))
	{
		AIChar->setMoveSpeed(AIChar -> RunSpeed);
	}

	UpdateChasingTarget();

	GetWorldTimerManager().SetTimer(ChasingTimer, this, &ABaseAIController::UpdateChasingTarget, 0.25f, true);
}


void ABaseAIController::UpdateChasingTarget()
{
	if (TargetActor && bIsChasing)
	{
		MoveToActor(TargetActor, 500.0f);
	}
}

void ABaseAIController::StopChasingTarget()
{
	if (!bIsChasing) return;
	
	TargetActor = nullptr;
		bIsChasing = false;
		
	GetWorldTimerManager().ClearTimer(ChasingTimer);

	StopMovement();

	if (ABaseAICharacter* AIChar = Cast<ABaseAICharacter>(GetPawn()))
	{
		AIChar->setMoveSpeed(AIChar->WalkSpeed);
	}
	
	GetWorldTimerManager().SetTimer(RandomMoveTimer, this, &ABaseAIController::MoveToRandomLocation, 3.0f, true, 2.0f);
}