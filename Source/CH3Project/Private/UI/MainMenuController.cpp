#include "UI/MainMenuController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"
#include "Character/CH3Character.h"
#include "Character/CH3PlayerController.h"

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
}

void AMainMenuController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//
	// if (!MenuCamera) return;
	//

	// // 3. 마우스 위치를 기반으로 카메라 위치 계산
	// float MouseX, MouseY;
	// GetMousePosition(MouseX, MouseY);
	//
	// // 아 ㅋㅋ 내문제 아니네 ~ㅋㅋㅋ
	//
	// // FVector2D ViewportSize; <- 변수 달라서 안됨 ㅅㄱ

	// int32 ViewportX = 0, ViewportY = 0;
	// GetViewportSize(ViewportX, ViewportY);
	//
	// if (ViewportY > 0 && ViewportX > 0)
	// {

	// 	// -1 to 1 범위로 정규화
	// 	const float NormalizedX = (MouseX / ViewportX) * 2.f - 1.f;
	// 	const float NormalizedY = (MouseY / ViewportY) * 2.f - 1.f;
	//
	// 	// 오프셋 계산 (언리얼 좌표계에 맞게 Y를 Z로, X를 Y로 매핑)
	// 	const FVector CameraOffset = FVector(-NormalizedX * MaxCameraOffset, 0.0f , -NormalizedY * MaxCameraOffset);
	// 	const FVector TargetLocation = InitialCameraTransform.GetLocation() + CameraOffset;
	//
	// 	// 4. 부드럽게 보간하여 위치 업데이트

	// 	const FVector NewLocation = FMath::VInterpTo(MenuCamera->GetActorLocation(), TargetLocation, DeltaTime, CameraInterpSpeed);
	// 	MenuCamera->SetActorLocation(NewLocation);
	// }
}