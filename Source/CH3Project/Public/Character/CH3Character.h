// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CH3Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;

struct FInputActionValue;

UCLASS()
class CH3PROJECT_API ACH3Character : public ACharacter
{
	GENERATED_BODY()

public:
	
	ACH3Character();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> CurrentWeapon; // 현재 장착된 무기의 클래스

	AWeapon* CurrentWeaponInstance; // 현재 장착된 무기의 인스턴스

	void Move(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<AWeapon> NewWeaponClass);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashSpeed; // 대시 속도
	float LastDashTime; // 마지막 대시 시간
	float DashCooldown; // 대시 쿨타임
	void FirePressed();
	void FireReleased();

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
