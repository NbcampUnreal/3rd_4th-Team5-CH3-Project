#include "UI/MainMenuController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"
#include "Character/CH3Character.h"
#include "Character/CH3PlayerController.h"

// AMainMenuController::AMainMenuController()
// {
// 	PrimaryActorTick.bCanEverTick = true;
// }

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 레벨에 있는 카메라 액터 찾기
	MenuCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
	if (MenuCamera)
	{
		// 카메라를 찾았을 때 출력
		UE_LOG(LogTemp, Error, TEXT("카메라 찾기 성공: %s"), *MenuCamera->GetName());
		InitialCameraTransform = MenuCamera->GetActorTransform();
		SetViewTargetWithBlend(MenuCamera, 1.f);

		// 헤더에서 쓴 false 를 true 로 활성화 시키는 매직~
		// 모든 카메라 설정이 성공적으로 끝나면 '준비 완료' 신호를 보냄
		bIsCameraInitialized = true;
	}
	else
	{
		// 카메라를 못 찾았을 때 출력
		UE_LOG(LogTemp, Error, TEXT("레벨에서 CameraActor를 찾지 못했습니다!"));
	}

	// 2. 위젯 생성 및 마우스 설정
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
		}
	}
	SetShowMouseCursor(true);
	SetInputMode(FInputModeGameAndUI());

	bShowMouseCursor = true; // 커서 표시
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false); 
	SetInputMode(InputMode);
	
}

void AMainMenuController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (!bIsCameraInitialized) return;
	
	if (!MenuCamera) return;

	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY))
	{
		return; // 마우스 좌표 못 얻으면 종료
	}

	int32 ViewportX = 0, ViewportY = 0;
	GetViewportSize(ViewportX, ViewportY);

	if (ViewportX <= 0 || ViewportY <= 0)
	{
		return;
	}

	// 뷰포트 안에 있는지 체크
	if (MouseX < 0 || MouseX > ViewportX || MouseY < 0 || MouseY > ViewportY)
	{
		// 마우스가 뷰포트 밖이면 카메라 중앙으로 복귀
		const FVector NewLocation = FMath::VInterpTo(
			MenuCamera->GetActorLocation(),
			InitialCameraTransform.GetLocation(),
			DeltaTime,
			CameraInterpSpeed
		);
		MenuCamera->SetActorLocation(NewLocation);
		return;
	}

	// 범위 제한 후 정규화
	MouseX = FMath::Clamp(MouseX, 0.f, (float)ViewportX);
	MouseY = FMath::Clamp(MouseY, 0.f, (float)ViewportY);

	const float NormalizedX = (MouseX / ViewportX) * 2.f - 1.f;
	const float NormalizedY = (MouseY / ViewportY) * 2.f - 1.f;

	const FVector CameraOffset = FVector(-NormalizedX * MaxCameraOffset, 0.f, -NormalizedY * MaxCameraOffset);
	const FVector TargetLocation = InitialCameraTransform.GetLocation() + CameraOffset;

	const FVector NewLocation = FMath::VInterpTo(MenuCamera->GetActorLocation(), TargetLocation, DeltaTime, CameraInterpSpeed);
	MenuCamera->SetActorLocation(NewLocation);
	
}