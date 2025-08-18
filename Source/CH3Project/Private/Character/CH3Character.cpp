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
ACH3Character::ACH3Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("무기 스폰에 실패했습니다! CurrentWeapon을 확인하세요.")); // 무기 스폰 실패 시 에러 로그 출력
	}
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
	PlayFireMontage(); // 발사 애니메이션 몽타주 재생
	// 단발 모드일 때만 즉시 발사
	if (CurrentWeaponInstance->GetFireMode() == EFireMode::SemiAuto) // 단발 모드일 때는 HandleFire를 즉시 호출
	{
		CurrentWeaponInstance->HandleFire();
	}
	// 연사 모드일 때는 StartFire/StopFire를 입력 이벤트에 맞게 따로 호출해야 함
	else if (CurrentWeaponInstance->GetFireMode() == EFireMode::FullAuto) // 연사 모드일 때는 StartFire를 호출
	{
		CurrentWeaponInstance->StartFire();
		// Release 이벤트가 발생하면 StopFire를 호출하도록 설정
	}
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
		// 플레이어의 마지막 입력 방향을 가져온다
		FVector DashDirection = MovementComponent->GetLastInputVector();

		// 입력이 없으면 함수를 즉시 종료하여 대시가 실행되지 않도록 합니다.
		if (DashDirection.IsNearlyZero())
		{
			return;
		}
		// 대시 방향의 Z축을 0으로 강제 설정한다
		DashDirection.Z = 0.0f;
		DashDirection.Normalize();
		FVector DashVelocity = DashDirection * DashSpeed;
		LaunchCharacter(DashVelocity, true, false);
		LastDashTime = CurrentTime;
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
	// 자신이 쏜 총알(Projectile, 무기 등)에 의한 데미지는 무시
	if (DamageCauser && DamageCauser->GetOwner() == this)
	{
		return 0.0f; // 데미지 무시
	}

	Health -= DamageAmount; // 피해량만큼 체력을 감소시킨다
	if (Health <= 0.0f) //체력이 0 이하가 되면 사망처리
	{
		Health = 0.0f;
		Destroy();
	}
	return DamageAmount; // 실제로 입은 피해량을 반환
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