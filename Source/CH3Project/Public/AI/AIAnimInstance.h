#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"


UCLASS()
class CH3PROJECT_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float speed;
	
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
