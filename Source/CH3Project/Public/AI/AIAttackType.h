#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIAttackType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_Melee UMETA(DisplayName = "Melee"),
	EAT_Ranged UMETA(DisplayName = "Ranged")

};
