// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CH3PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CH3PROJECT_API ACH3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACH3PlayerController();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UiPauseAction;


	void MouseLook();
	void PlayerTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="UI|Pause")
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable, Category="UI|Pause")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category="UI|Pause")
	void HidePauseMenu();

	UFUNCTION(BlueprintPure, Category="UI|Pause")
	bool IsPauseMenuVisible() const;

protected:
	// 일시정지 메뉴 UMG 클래스(BP에서 지정)
	UPROPERTY(EditDefaultsOnly, Category="UI|Pause")
	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY(Transient)
	TObjectPtr<class UUserWidget> PauseMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category="UI|Pause")
	int32 PauseMenuZOrder = 100;

	// 싱글/로컬은 true 권장, 멀티는 보통 false(UI-Only)
	UPROPERTY(EditDefaultsOnly, Category="UI|Pause")
	bool bUseGamePaused = true;

	// 닫을 때 위젯 파괴할지 여부
	UPROPERTY(EditDefaultsOnly, Category="UI|Pause")
	bool bDestroyOnHide = false;

private:
	void ApplyUIInputMode();
	void ApplyGameInputMode();
	void SetPausedState(bool bPaused);
};
