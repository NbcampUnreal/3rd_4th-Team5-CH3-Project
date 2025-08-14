
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
	// NativeConstruct() -> 생성자임
	// virtual void NativeConstruct() override;

	//-WBP_Character_Stats
	UFUNCTION(BlueprintCallable)
	void SetProgressHPBar(int HP);
	
	//-WBP_Character_Stats
	UFUNCTION(BlueprintCallable)
	void SetProgressSTBar(int ST);
	
	// 최소값 0.0f 최대값 1.0f -> 프로그래스바가 최소 최대값이 0에서 1로 정해져 있기때문에 체력수치랑 다르게 설정해야된다.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HP_Bar; // UI_체력바
	
	// 최소값 0.0f 최대값 1.0f -> 프로그래스바가 최소 최대값이 0에서 1로 정해져 있기때문에 체력수치랑 다르게 설정해야된다.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_ST_Bar; // UI_스테미너바
};
