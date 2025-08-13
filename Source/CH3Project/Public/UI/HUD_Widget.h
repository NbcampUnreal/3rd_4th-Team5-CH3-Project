#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HUD_Widget.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API UHUD_Widget : public UUserWidget
{
	GENERATED_BODY()

	// 최소값 0.0f 최대값 1.0f -> 프로그래스바가 최소 최대값이 0에서 1로 정해져 있기때문에 체력수치랑 다르게 설정해야된다.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HP_Bar; // UI_체력바
	
	// 최소값 0.0f 최대값 1.0f -> 프로그래스바가 최소 최대값이 0에서 1로 정해져 있기때문에 체력수치랑 다르게 설정해야된다.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_ST_Bar; // UI_스테미너바
	
};
