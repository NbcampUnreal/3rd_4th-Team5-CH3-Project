// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/Weapon.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "TestCharacter.generated.h"



UCLASS()
class CH3PROJECT_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATestCharacter();

	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeapon> CurrentWeapon;

	//인핸스드 인풋 시스템으로 Fire, Move, Look 시스템의 테스트용 코드임.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;


public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	virtual void EquipWeapon(AWeapon* WeaponToEquip);

	void FireWeapon();
	void StopFireWeapon();
	virtual FVector GetAimTargetLocation() const;

	UFUNCTION()
	void OnReload();


};
