// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/Weapon.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "Item/ItemWeapon.h"
#include "TestCharacter.generated.h"


class AWeapon;

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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") //AI, 캐릭터에서 스폰 시 자동으로 얻을 수 있는 무기를 선택하기 위함
	TSubclassOf<AWeapon> WeaponClass;

	//인핸스드 인풋 시스템으로 Fire, Move, Look 시스템의 테스트용 코드임.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;



	//인벤토리 스크롤 기능 테스트 중
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ScrollAction;
	
	void AutoEquipWeapon();

	//인벤토리 테스트 중

	UPROPERTY()
	TArray<AWeapon*> Inventory;



	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocketName = "RightHandSocket";






public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	

	//인벤토리 기능 테스트 중입니다.
	void AddWeaponToInventory(AWeapon* NewWeapon);
	void RemoveWeaponFromInventory(AWeapon* WeaponToRemove);
	virtual void EquipWeapon(AWeapon* WeaponToEquip);
	void SelectWeaponByIndex(int32 Index);
	
	void CycleWeaponUp();     // 휠 업
	void CycleWeaponDown();   // 휠 다운


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 CurrentWeaponIndex = 0;
	UFUNCTION()
	void HandleMouseScroll(const FInputActionValue& Value);

	
	



	void FireWeapon();
	void StopFireWeapon();
	
	virtual FVector GetAimTargetLocation() const;

	UFUNCTION()
	void OnReload();

private:
	bool bIsFiring = false;


};
