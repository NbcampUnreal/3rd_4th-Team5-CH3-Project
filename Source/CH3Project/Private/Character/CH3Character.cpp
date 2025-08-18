// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/CH3PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Weapon.h"
#include "UI/MainMenuController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"
#include <UI/Character_Stats_Widget.h>

void ACH3Character::EnsureCreateHUDs()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !PC->IsLocalController())
	{
		return;
	}

	// 다중 스펙 생성
	for (const FHUDWidgetSpec& Spec : HUDWidgetsToCreate)
	{
		if (!Spec.Class || Spec.Name.IsNone())
		{
			continue;
		}
		if (HUDWidgetInstances.Contains(Spec.Name))
		{
			continue; // 이미 생성됨
		}

		if (UUserWidget* W = CreateWidget<UUserWidget>(PC, Spec.Class))
		{
			W->AddToViewport(Spec.ZOrder);
			HUDWidgetInstances.Add(Spec.Name, W);
		}
	}

	// 기존 단일 HUD도 필요 시 생성
	if (HUDWidgetClass && !HUDWidgetInstance)
	{
		if (UUserWidget* W = CreateWidget<UUserWidget>(PC, HUDWidgetClass))
		{
			W->AddToViewport();
			HUDWidgetInstance = W;
		}
	}
}

ACH3Character::ACH3Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bDoCollisionTest = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	DashSpeed = 2000.0f; // DashSpeed 초기화
	LastDashTime = -FLT_MAX; // FLT_MAX는 부동 소수점의 최대값으로 LastDashTime을 -FLT_MAX로 설정함으로써 캐릭터가 게임 시작 시 즉시 대시를 할 수 있도록 함
	DashCooldown = 1.0f; // 1초 쿨타임

	//// 총알의 콜리전 채널(WorldDynamic)을 Ignore로 설정
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
}

void ACH3Character::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentWeapon)
	{
		CurrentWeaponInstance = GetWorld()->SpawnActor<AWeapon>(CurrentWeapon);
	}

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None); // 무기 뼈대를 숨김

	if (CurrentWeaponInstance) // 무기 인스턴스가 성공적으로 생성되었는지 확인
	{
		CurrentWeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket")); // 무기를 캐릭터의 손에 붙임
		CurrentWeaponInstance->SetOwner(this); // 무기의 소유자를 현재 캐릭터로 설정
		CurrentWeaponInstance->SetActorRelativeScale3D(FVector(0.7f)); // 무기의 크기를 조정 (필요에 따라 조정 가능)
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("무기 스폰에 실패했습니다! CurrentWeapon을 확인하세요.")); // 무기 스폰 실패 시 에러 로그 출력
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->IsLocalController() && HUDWidgetClass)
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();
			}
		}
	}

	// BeginPlay에서도 시도(이미 소유 중인 경우)
	EnsureCreateHUDs();
}

void ACH3Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnsureCreateHUDs();
}

void ACH3Character::OnRep_Controller()
{
	Super::OnRep_Controller();
	EnsureCreateHUDs();
}

UUserWidget* ACH3Character::GetHUDWidgetByName(FName Name) const
{
	if (const TObjectPtr<UUserWidget>* Found = HUDWidgetInstances.Find(Name))
	{
		return Found->Get();
	}
	return nullptr;
}

void ACH3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ACH3PlayerController* MyController = Cast<ACH3PlayerController>(GetController()))
		{
			if (MyController->MoveAction)
			{
				EnhancedInputComponent->BindAction(MyController->MoveAction, ETriggerEvent::Triggered, this, &ACH3Character::Move);
			}
			if (MyController->FireAction)
			{
				EnhancedInputComponent->BindAction(MyController->FireAction, ETriggerEvent::Started, this, &ACH3Character::Fire); // 단발 모드에서 즉시 발사

				EnhancedInputComponent->BindAction(MyController->FireAction, ETriggerEvent::Completed, this, &ACH3Character::FireReleased); // 연사 모드에서 StopFire를 호출하기 위해 Completed 이벤트도 바인딩
			}
			if (MyController->DashAction)
			{
				EnhancedInputComponent->BindAction(MyController->DashAction, ETriggerEvent::Triggered, this, &ACH3Character::Dash);
			}
			if(MyController->FireModeAction)
			{
				EnhancedInputComponent->BindAction(MyController->FireModeAction, ETriggerEvent::Triggered, this, &ACH3Character::FireModeChanged);
			}
			/*if(MyController->UiPauseAction)
			{
				EnhancedInputComponent->BindAction(MyController->UiPauseAction, ETriggerEvent::Triggered, this, &ACH3Character::UiPause);
			}*/
		}
	}

}

void ACH3Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 탑다운뷰 에서의 이동 방향을 카메라의 방향에 맞춰 조정
	AddMovementInput(CameraComp->GetForwardVector(), MovementVector.X);
	AddMovementInput(CameraComp->GetRightVector(), MovementVector.Y);
}
void ACH3Character::Fire(const FInputActionValue& Value)
{
	if (!CurrentWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("무기가 장착되어 있지 않습니다."));
		return;
	}

	// 사격 시작: 이동속도 감소(Started 시 1회만)
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		if (!bIsFiring)
		{
			WalkSpeedBeforeFire = Move->MaxWalkSpeed;
			Move->MaxWalkSpeed = WalkSpeedBeforeFire * FiringSpeedMultiplier;
			bIsFiring = true;
		}
	}

	PlayFireMontage(); // 발사 애니메이션 몽타주 재생

	// 단발/연사 처리
	if (CurrentWeaponInstance->GetFireMode() == EFireMode::SemiAuto)
	{
		CurrentWeaponInstance->HandleFire();
	}
	else if (CurrentWeaponInstance->GetFireMode() == EFireMode::FullAuto)
	{
		CurrentWeaponInstance->StartFire();
	}
	PlayAnimMontage(FireAnimMontage); // 발사 애니메이션 몽타주 재생

}

void ACH3Character::FireReleased()
{
	if (!CurrentWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("무기가 장착되어 있지 않습니다."));
		return;
	}
	// 연사 모드일 때 StopFire 호출
	if (CurrentWeaponInstance->GetFireMode() == EFireMode::FullAuto)
	{
		CurrentWeaponInstance->StopFire();
	}

	// 사격 종료: 이동속도 복구(Completed)
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		if (bIsFiring)
		{
			Move->MaxWalkSpeed = WalkSpeedBeforeFire;
			bIsFiring = false;
		}
	}
	
}

void ACH3Character::Dash(const FInputActionValue& Value)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();

    if (CurrentTime - LastDashTime < DashCooldown)
    {
        return;
    }
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent && MovementComponent->IsMovingOnGround())
    {
        FVector DashDirection = MovementComponent->GetLastInputVector();
        if (DashDirection.IsNearlyZero())
        {
            return;
        }
        DashDirection.Z = 0.0f;
        DashDirection.Normalize();
        FVector DashVelocity = DashDirection * DashSpeed;
        LaunchCharacter(DashVelocity, true, false);
        LastDashTime = CurrentTime;

        UpdateStatsUI();           // 즉시 0으로 초기화 반영
        StartStatsUIAutoUpdate();  // 쿨타임 동안 주기적으로 UI 갱신
    }
}


void ACH3Character::EquipWeapon(TSubclassOf<AWeapon> NewWeaponClass)
{
	if (NewWeaponClass)
	{
		if (CurrentWeaponInstance) // 기존 무기가 있다면
		{
			CurrentWeaponInstance->Destroy(); // 파괴합니다.
			CurrentWeaponInstance = nullptr; // 포인터를 nullptr로 초기화합니다.
		}
		// 새로운 무기 인스턴스를 스폰하여 CurrentWeaponInstance에 할당
		CurrentWeaponInstance = GetWorld()->SpawnActor<AWeapon>(NewWeaponClass);
			// 새로 생성된 무기의 콜리전을 비활성화하여 루프를 방지
			if (CurrentWeaponInstance->GetMesh())
			{
				CurrentWeaponInstance->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			CurrentWeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
			CurrentWeaponInstance->SetOwner(this);
			GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
			// 무기 크기 조정
			CurrentWeaponInstance->SetActorRelativeScale3D(FVector(0.7f));
	}
}

void ACH3Character::FireModeChanged(const FInputActionValue& Value)
{
	if (CurrentWeaponInstance)
	{
		// 현재 무기의 발사 모드를 변경
		if (CurrentWeaponInstance->GetFireMode() == EFireMode::SemiAuto)
		{
			CurrentWeaponInstance->SetFireMode(EFireMode::FullAuto);
			UE_LOG(LogTemp, Warning, TEXT("발사 모드가 연사로 변경되었습니다."));
		}
		else if (CurrentWeaponInstance->GetFireMode() == EFireMode::FullAuto)
		{
			CurrentWeaponInstance->SetFireMode(EFireMode::SemiAuto);
			UE_LOG(LogTemp, Warning, TEXT("발사 모드가 단발로 변경되었습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("무기가 장착되어 있지 않습니다."));
	}
}

float ACH3Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser && DamageCauser->GetOwner() == this)
	{
		return 0.0f;
	}

	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	UpdateStatsUI(); // 체력 변경 즉시 UI 갱신

	if (Health <= 0.0f)
	{
		Health = 0.0f;
		Destroy();
	}
	return DamageAmount;
}

void ACH3Character::PlayFireMontage()
{
	if (FireAnimMontage && GetMesh())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireAnimMontage); // 몽타주를 재생합니다.
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("애니메이션 인스턴스가 유효하지 않습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("발사 애니메이션 몽타주가 설정되지 않았습니다."));
	}
}

//void ACH3Character::UiPause(const FInputActionValue& Value)
//{
//	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
//	{
//		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(PlayerController)) // 메뉴호출 함수 만들어줘야함
//		{
//			MainMenuController->TogglePauseMenu(); // 메인 메뉴 컨트롤러의 토글 함수 호출
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("메인 메뉴 컨트롤러가 유효하지 않습니다."));
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("플레이어 컨트롤러가 유효하지 않습니다."));
//	}
//}

FVector ACH3Character::GetAimTargetLocation(float TraceDistance) const
{
	// 플레이어 컨트롤러를 통해 마우스 커서 기준으로 조준 지점 계산
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector2D MousePos;
		// 마우스 좌표를 얻지 못하면 화면 중앙을 사용(패드/고정 시점 대비)
		if (!PC->GetMousePosition(MousePos.X, MousePos.Y))
		{
			int32 SizeX = 0, SizeY = 0;
			PC->GetViewportSize(SizeX, SizeY);
			MousePos = FVector2D(SizeX * 0.5f, SizeY * 0.5f);
		}

		FVector WorldOrigin, WorldDir;
		if (PC->DeprojectScreenPositionToWorld(MousePos.X, MousePos.Y, WorldOrigin, WorldDir))
		{
			const FVector TraceStart = WorldOrigin;
			const FVector TraceEnd = TraceStart + WorldDir * TraceDistance;

			FHitResult Hit;
			FCollisionQueryParams Params(SCENE_QUERY_STAT(GetAimTarget), /*bTraceComplex*/ true);
			Params.AddIgnoredActor(this);
			if (CurrentWeaponInstance)
			{
				Params.AddIgnoredActor(CurrentWeaponInstance);
			}

			if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
			{
				return Hit.ImpactPoint;
			}
			return TraceEnd;
		}
	}

	// Fallback: 카메라(또는 액터) 전방으로 고정 거리
	const float FallbackDistance = TraceDistance > 0.f ? TraceDistance : 100000.f;
	const FVector Start = CameraComp ? CameraComp->GetComponentLocation() : GetActorLocation();
	const FVector Dir = CameraComp ? CameraComp->GetForwardVector() : GetActorForwardVector();
	return Start + Dir * FallbackDistance;
}

void ACH3Character::StartStatsUIAutoUpdate()
{
	// HUD가 아직 없으면 시작하지 않음(생성 후 BeginPlay/EnsureCreateHUDs에서 다시 호출)
	const bool bHasAnyHUD = (HUDWidgetInstance != nullptr) || (HUDWidgetInstances.Num() > 0);
	if (!bHasAnyHUD)
	{
		return;
	}

	if (!GetWorldTimerManager().IsTimerActive(StatsUITimerHandle))
	{
		// 최초 1회 즉시 동기화
		UpdateStatsUI();

		// 주기적 갱신 시작
		GetWorldTimerManager().SetTimer(
			StatsUITimerHandle,
			this, &ACH3Character::UpdateStatsUI,
			FMath::Max(0.01f, StatsUIUpdateInterval), /*bLoop*/ true);
	}
}

void ACH3Character::StopStatsUIAutoUpdate()
{
	if (GetWorldTimerManager().IsTimerActive(StatsUITimerHandle))
	{
		GetWorldTimerManager().ClearTimer(StatsUITimerHandle);
	}
}

// 필요 시 자동 중지를 원한다면(UpdateStatsUI 끝에서 호출)
void ACH3Character::UpdateStatsUI()
{
	// HP 퍼센트
	const float HPPercent = (MaxHealth > 0.f) ? FMath::Clamp(Health / MaxHealth, 0.f, 1.f) : 0.f;

	// 대시 쿨다운 퍼센트(1=준비완료)
	float CooldownPercent = 1.f;
	if (DashCooldown > 0.f)
	{
		const float Elapsed = GetWorld()->GetTimeSeconds() - LastDashTime;
		CooldownPercent = FMath::Clamp(Elapsed / DashCooldown, 0.f, 1.f);
	}

	// UI 반영(Stats 위젯 직접 사용 중이면 그에 맞게 호출)
	if (UCharacter_Stats_Widget* HUD = Cast<UCharacter_Stats_Widget>(HUDWidgetInstance))
	{
		HUD->SetProgressHPPercent(HPPercent);
		HUD->SetProgressSTPercent(CooldownPercent);
	}
	for (const TPair<FName, TObjectPtr<UUserWidget>>& Pair : HUDWidgetInstances)
	{
		if (UCharacter_Stats_Widget* HUD2 = Cast<UCharacter_Stats_Widget>(Pair.Value))
		{
			HUD2->SetProgressHPPercent(HPPercent);
			HUD2->SetProgressSTPercent(CooldownPercent);
		}
	}

	// 선택: 값이 변하지 않고(에러바 이내) 대시가 100%면 타이머 자동 중지
	constexpr float Epsilon = 0.001f;
	const bool bHPUnchanged   = FMath::IsNearlyEqual(LastHPPercentSent,   HPPercent, Epsilon);
	const bool bDashReady     = FMath::IsNearlyEqual(CooldownPercent, 1.f, Epsilon);
	const bool bDashUnchanged = FMath::IsNearlyEqual(LastDashPercentSent, CooldownPercent, Epsilon);

	if (!bHPUnchanged || !bDashUnchanged)
	{
		LastHPPercentSent   = HPPercent;
		LastDashPercentSent = CooldownPercent;
	}

	// 체력 변화가 없고, 대시가 준비(1.0) 상태면 굳이 타이머를 계속 돌릴 필요 없음
	if (bHPUnchanged && bDashReady)
	{
		StopStatsUIAutoUpdate();
	}
}