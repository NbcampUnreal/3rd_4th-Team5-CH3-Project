#include "UI/MainMenuController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"
#include "Character/CH3Character.h"
#include "Character/CH3PlayerController.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	// 1. ������ �ִ� ī�޶� ���� ã��
	MenuCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
	if (MenuCamera)
	{
		// ī�޶� ã���� �� ���
		UE_LOG(LogTemp, Error, TEXT("ī�޶� ã�� ����: %s"), *MenuCamera->GetName());

		InitialCameraTransform = MenuCamera->GetActorTransform();
		SetViewTargetWithBlend(MenuCamera, 1.f);
	}
	else
	{
		// ī�޶� �� ã���� �� ���
		UE_LOG(LogTemp, Error, TEXT("�������� CameraActor�� ã�� ���߽��ϴ�!"));
	}

	// 2. ���� ���� �� ���콺 ����
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
	// // 3. ���콺 ��ġ�� ������� ī�޶� ��ġ ���
	// float MouseX, MouseY;
	// GetMousePosition(MouseX, MouseY);
	//
	// // �� ���� ������ �ƴϳ� ~������
	//
	// // FVector2D ViewportSize; <- ���� �޶� �ȵ� ����
	// int32 ViewportX = 0, ViewportY = 0;
	// GetViewportSize(ViewportX, ViewportY);
	//
	// if (ViewportY > 0 && ViewportX > 0)
	// {
	// 	// -1 to 1 ������ ����ȭ
	// 	const float NormalizedX = (MouseX / ViewportX) * 2.f - 1.f;
	// 	const float NormalizedY = (MouseY / ViewportY) * 2.f - 1.f;
	//
	// 	// ������ ��� (�𸮾� ��ǥ�迡 �°� Y�� Z��, X�� Y�� ����)
	// 	const FVector CameraOffset = FVector(-NormalizedX * MaxCameraOffset, 0.0f , -NormalizedY * MaxCameraOffset);
	// 	const FVector TargetLocation = InitialCameraTransform.GetLocation() + CameraOffset;
	//
	// 	// 4. �ε巴�� �����Ͽ� ��ġ ������Ʈ
	// 	const FVector NewLocation = FMath::VInterpTo(MenuCamera->GetActorLocation(), TargetLocation, DeltaTime, CameraInterpSpeed);
	// 	MenuCamera->SetActorLocation(NewLocation);
	// }
}