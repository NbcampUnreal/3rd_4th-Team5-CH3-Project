
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "HUD_Widget.generated.h"

class UTextBlock;
class UProgressBar;


/**
 * 
 */
UCLASS()
class CH3PROJECT_API UHUD_Widget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HUD_Timer;

	// 타이머 함수 명칭만 선언
	// UFUNCTION(BlueprintCallable)
	// void SetTextHUD_Timer(FText Time);
};
