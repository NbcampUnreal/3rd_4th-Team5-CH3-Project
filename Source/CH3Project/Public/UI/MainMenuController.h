#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

/**
 메인 화면에서만 작동될 컨트롤러 헤더파일입니다. 아직 개념 공부중..
 */
UCLASS()
class CH3PROJECT_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	
	// 메인메뉴위젯 블루프린트 담는 변수
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	class UUserWidget* MainMenuWidgetInstance;

	// 제어할 카메라와 초기 트랜스폼
	UPROPERTY()
	class ACameraActor* MenuCamera;
	
	FTransform InitialCameraTransform;

	// 카메라 이동 관련 설정값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxCameraOffset = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraInterpSpeed = 5.0f;
};
