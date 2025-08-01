
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAuto	UMETA(DisplayName = "Semi-Automatic"),
	FullAuto	UMETA(DisplayName = "Fully Automatic")
};

UCLASS()
class CH3PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> MuzzleOffset; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FireMontage;


	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USphereComponent> PickupTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EFireMode FireMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ABullet> BulletActor; // 총알 액터 클래스를 저장하는 변수


	bool bIsReloading = false; // 사격 중 재장전 여부

	FTimerHandle AutoFireHandle;
	FTimerHandle ReloadHandle;

public:	
	virtual void Tick(float DeltaTime) override; 

	void StartFire(); // 사격 시작
	void StopFire();
	void HandleFire(); // 사격 처리 함수, 자식 클래스에서 오버라이드 가능

	virtual void Reload(); // 재장전 시작 함수, 자식 클래스에서 오버라이드 가능
	virtual void FinishReload(); // 재장전 완료 처리 함수

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, // 오버랩을 통돌 충돌 처리
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); // 오버랩 시작 시 호출되는 함수

	void SetMaxAmmo(int32 Value); // 최대 탄약 설정 함수
	void SetReloadTime(float Time); // 재장전 시간 설정 함수
	void SetFireMode(EFireMode Mode); // 사격 모드 설정 함수
	void SetBullet(TSubclassOf<ABullet> Bullet); // 총알 설정 함수

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
};

