#include "AI/BaseAICharacter.h"
#include "AI/BaseAIController.h"
#include "AI/AIHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	


ABaseAICharacter::ABaseAICharacter()
{
	AIControllerClass = ABaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;	
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	HealthComponent = CreateDefaultSubobject<UAIHealthComponent>(TEXT("HealthComponent"));
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AI Character %s has begun play."), *GetName());

}

void ABaseAICharacter::setMoveSpeed(float NewSpeed)
{
	if(UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("AI Character %s speed set to: %f"), *GetName(), NewSpeed);
	}
}

void ABaseAICharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Performs MELEE ATTACK!"), *GetName());
}

void ABaseAICharacter::PerformRangedAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Performs RANGED ATTACK!"), *GetName());
}
