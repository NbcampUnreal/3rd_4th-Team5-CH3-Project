#include "AI/BaseAICharacter.h"
#include "AI/BaseAIController.h"
#include "AI/AIHealthComponent.h"
#include "AI/AIWeaponActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BrainComponent.h"



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

	if (WeaponClass)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AAIWeaponActor>(WeaponClass);
		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		}
	}

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


void ABaseAICharacter::HandleDeath()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && AIController->GetBrainComponent())
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetSimulatePhysics(true);

	SetLifeSpan(10.0f); 
}
