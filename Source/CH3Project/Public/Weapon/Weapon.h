
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FireMontage;


	UPROPERTY(VisibleAnywhere, Category = "Weapon") //무기 습득 오버랩 이벤트를 위한 SphereComponent
	TObjectPtr<class USphereComponent> PickupTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EFireMode FireMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	/** 내부 설정 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ABullet> BulletActor;

	/** 상태 */
	bool bIsReloading = false;

	FTimerHandle AutoFireHandle;

public:	
	virtual void Tick(float DeltaTime) override;

	/** 외부에서 발사 호출용 */
	void StartFire();
	void StopFire();
	virtual void HandleFire();

	void Reload();
	void FinishReload();

	/*무기 줍기*/
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 자식에서 수치 설정용 */
	void SetMaxAmmo(int32 Value);
	void SetReloadTime(float Time);
	void SetFireMode(EFireMode Mode);
	void SetBullet(TSubclassOf<ABullet> Bullet);
};

