// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestCharacter.h"
#include "InputActionValue.h"
#include "TestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API ATestPlayer : public ATestCharacter
{
	GENERATED_BODY()

public:
	ATestPlayer();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;

	
	// 입력 처리 함수
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

	//사격 함수
	void FireWeapon();
	void StopFireWeapon();
};