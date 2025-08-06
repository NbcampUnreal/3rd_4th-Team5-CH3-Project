#include "AI/AIAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		speed = OwningActor->GetVelocity().Size();

	}
}
