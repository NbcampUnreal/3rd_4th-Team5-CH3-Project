
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


	UPROPERTY(VisibleAnywhere, Category = "Weapon") //���� ���� ������ �̺�Ʈ�� ���� SphereComponent
	TObjectPtr<class USphereComponent> PickupTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EFireMode FireMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	/** ���� ���� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ABullet> BulletActor;

	/** ���� */
	bool bIsReloading = false;

	FTimerHandle AutoFireHandle;

public:	
	virtual void Tick(float DeltaTime) override;

	/** �ܺο��� �߻� ȣ��� */
	void StartFire();
	void StopFire();
	virtual void HandleFire();

	void Reload();
	void FinishReload();

	/*���� �ݱ�*/
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** �ڽĿ��� ��ġ ������ */
	void SetMaxAmmo(int32 Value);
	void SetReloadTime(float Time);
	void SetFireMode(EFireMode Mode);
	void SetBullet(TSubclassOf<ABullet> Bullet);
};

