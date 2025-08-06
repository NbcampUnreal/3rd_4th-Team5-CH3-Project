#include "AI/AIHealthComponent.h"
#include "AI/BaseAICharacter.h"	



UAIHealthComponent::UAIHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAIHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;	

	AActor* MyOwner = GetOwner();
	if(MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UAIHealthComponent::HandleTakeAnyDamage);
	}
}

void UAIHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("%s's Health: %f"), *GetOwner()->GetName(), CurrentHealth);

	if(CurrentHealth <= 0.0f)
	{

		ABaseAICharacter* OwningCharacter = Cast<ABaseAICharacter>(GetOwner());
		if (OwningCharacter)
		{
			OwningCharacter->HandleDeath();
		}

		UE_LOG(LogTemp, Warning, TEXT("%s has died."), *GetOwner()->GetName());
	}	
}