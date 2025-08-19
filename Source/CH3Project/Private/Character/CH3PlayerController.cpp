// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

ACH3PlayerController::ACH3PlayerController() : DefaultMappingContext(nullptr), MoveAction(nullptr), 
	EquipAction(nullptr), FireAction(nullptr), DashAction(nullptr), FireModeAction(nullptr), UiPauseAction(nullptr),
	PauseMenuInstance(nullptr), PauseMenuZOrder(100), bDestroyOnHide(false), bUseGamePaused(true)
{
	bShowMouseCursor = true; // 마우스 커서를 보이게 설정
	bEnableClickEvents = false; // 클릭 이벤트 비활성화
	bEnableMouseOverEvents = false; // 마우스 오버 이벤트 비활성화
	
}

void ACH3PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				
			}
		}
	}
}

void ACH3PlayerController::MouseLook() // 마우스 위치를 바라보도록 플레이어 폰을 회전시키는 함수
{
	// 일시정지/UI 표시/입력 차단 시에는 회전시키지 않음
	if (IsPauseMenuVisible() || UGameplayStatics::IsGamePaused(GetWorld()) || IsLookInputIgnored())
	{
		return;
	}

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		if (APawn* const PlayerPawn = GetPawn())
		{
			const FVector PawnLoc = PlayerPawn->GetActorLocation();
			const FVector Target(HitResult.Location.X, HitResult.Location.Y, PawnLoc.Z);
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PawnLoc, Target);
			PlayerPawn->SetActorRotation(LookAtRotation);
		}
	}
}

void ACH3PlayerController::PlayerTick(float DeltaTime) // 플레이어 컨트롤러의 틱 함수
{
	Super::PlayerTick(DeltaTime);
	
	MouseLook(); // 마우스 위치를 바라보도록 플레이어 폰을 회전시킴
	
}

void ACH3PlayerController::TogglePauseMenu()
{
	if (IsPauseMenuVisible())
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

bool ACH3PlayerController::IsPauseMenuVisible() const
{
	return PauseMenuInstance && PauseMenuInstance->IsInViewport();
}

void ACH3PlayerController::ShowPauseMenu()
{
	if (!IsLocalController()) return;

	if (!PauseMenuInstance)
	{
		if (!PauseMenuClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("PauseMenuClass not set on CH3PlayerController"));
			return;
		}
		PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
		if (!PauseMenuInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create PauseMenu widget"));
			return;
		}
	}

	if (!PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->AddToViewport(PauseMenuZOrder);
	}

	ApplyUIInputMode();
	SetPausedState(true);
}

void ACH3PlayerController::HidePauseMenu()
{
	if (!IsLocalController()) return;

	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
	}
	if (bDestroyOnHide)
	{
		PauseMenuInstance = nullptr;
	}

	SetPausedState(false);
	ApplyGameInputMode();
	FlushPressedKeys();

	// Enhanced Input 매핑 재빌드(입력이 죽는 경우 방지)
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = true;
			Subsystem->RequestRebuildControlMappings(Options);
		}
	}

	UE_LOG(
		LogTemp, Log,
		TEXT("Resume -> Paused:%d Cursor:%d MoveIgnored:%d LookIgnored:%d"),
		UGameplayStatics::IsGamePaused(GetWorld()) ? 1 : 0,
		bShowMouseCursor ? 1 : 0,
		IsMoveInputIgnored() ? 1 : 0,
		IsLookInputIgnored() ? 1 : 0
	);
}

void ACH3PlayerController::ApplyUIInputMode()
{
	// UI 전용 입력, 마우스 커서 표시 + 클릭/오버 이벤트 활성화
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// 포커스 위젯을 전달하지 않아 Non-Focusable 오류 회피
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
		this,
		/*InWidgetToFocus=*/ nullptr,
		EMouseLockMode::DoNotLock,
		/*bFlushInput=*/ true);

	// 게임 입력 차단
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
}

void ACH3PlayerController::ApplyGameInputMode()
{
	// 게임 입력 복원
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);

	// 커서 표시, UI 클릭/오버 비활성
	bShowMouseCursor = true;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	// 첫 클릭 캡처 소비 금지
	FInputModeGameOnly Mode;
	Mode.SetConsumeCaptureMouseDown(false);
	SetInputMode(Mode); // 중요: 아래 SetInputMode_GameOnly 호출 제거

	UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void ACH3PlayerController::SetPausedState(bool bPaused)
{
	if (bUseGamePaused)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), bPaused);
	}
}