#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Character_Stats_Widget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

/**
 * 
 */
UCLASS()
class CH3PROJECT_API UCharacter_Stats_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 초기 바 값 세팅이 필요하면 활성화
	virtual void NativeConstruct() override;

	// WBP_Character_Stats - 정수(0~100 가정) 입력용
	UFUNCTION(BlueprintCallable)
	void SetProgressHPBar(int HP);

	// WBP_Character_Stats - 정수(0~100 가정) 입력용
	UFUNCTION(BlueprintCallable)
	void SetProgressSTBar(int ST);

	// 퍼센트(0~1) 직접 설정용 - HUD/캐릭터에서 권장 사용
	UFUNCTION(BlueprintCallable)
	void SetProgressHPPercent(float HPPercent);

	UFUNCTION(BlueprintCallable)
	void SetProgressSTPercent(float STPercent);

	// 최소값 0.0f 최대값 1.0f
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HP_Bar; // UI_체력바

	// 최소값 0.0f 최대값 1.0f
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_ST_Bar; // UI_스테미너/대시 쿨타임 바
};
