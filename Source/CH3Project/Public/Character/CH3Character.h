// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CH3Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class UUserWidget; // 추가
struct FInputActionValue;

// 여러 HUD 위젯을 지정하기 위한 스펙
USTRUCT(BlueprintType)
struct FHUDWidgetSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FName Name = NAME_None; // 고유 키(중복 금지)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> Class = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	int32 ZOrder = 0;
};

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
	void Fire(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void FireModeChanged(const FInputActionValue& Value);
	void UiPause(const FInputActionValue& Value);


	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<AWeapon> NewWeaponClass);
	void FireReleased();
	void FireTriggered(const FInputActionValue& Value); // ← 추가
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser); // 캐릭터가 피해를 입었을 때 호출되는 함수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashSpeed; // 대시 속도
	float LastDashTime; // 마지막 대시 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown; // 대시 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float Health = 100.0f;

	// 최대 체력 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float MaxHealth = 100.0f;

	void PlayFireMontage(); // 애니메이션 몽타주 재생 함수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> FireAnimMontage; // 발사 애니메이션 몽타주

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 마우스 커서 기준 조준 지점(월드 좌표) 계산
	UFUNCTION(BlueprintCallable, Category = "Aim")
	FVector GetAimTargetLocation(float TraceDistance = 100000.f) const;

	// 사격 시 이동속도 제어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float FiringSpeedMultiplier = 0.5f; // 사격 중 속도 배수(0.5 = 50%)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsFiring = false; // 사격 입력 유지 중 여부

	UPROPERTY(Transient)
	float WalkSpeedBeforeFire = 0.f; // 사격 전 이동 속도 저장용 

	// HUD 블루프린트(WBP_HUD)를 할당할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	// 생성된 HUD 인스턴스(필요 시 접근)
	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<UUserWidget> HUDWidgetInstance = nullptr;

	// 다수의 HUD 위젯을 한 번에 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TArray<FHUDWidgetSpec> HUDWidgetsToCreate;

	// 생성된 위젯 인스턴스들(Name -> Instance)
	UPROPERTY(BlueprintReadOnly, Category="UI")
	TMap<FName, TObjectPtr<UUserWidget>> HUDWidgetInstances;

	UFUNCTION(BlueprintCallable, Category="UI")
	UUserWidget* GetHUDWidgetByName(FName Name) const;

	void UpdateStatsUI(); // 체력, 대시 쿨타임 등 UI 갱신

protected:
	// 소유/복제 시점에도 HUD를 보장적으로 생성
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

	// 중복 생성 방지 포함 생성 헬퍼
	void EnsureCreateHUDs();

	// 업데이트 주기(초). 필요에 따라 조정(0.05f = 20Hz)
	UPROPERTY(EditAnywhere, Category="UI")
	float StatsUIUpdateInterval = 0.05f;

	FTimerHandle StatsUITimerHandle;

	void StartStatsUIAutoUpdate();
	void StopStatsUIAutoUpdate();

	// 선택: 자동 중지 기준을 위해 마지막 전송 값 캐시(미세 변경 무시용)
	float LastHPPercentSent = -1.f;
	float LastDashPercentSent = -1.f;

	// 발사 입력 보조 상태
	UPROPERTY(Transient)
	bool bFireHeld = false; // 이미 선언되어 있으면 유지
};
