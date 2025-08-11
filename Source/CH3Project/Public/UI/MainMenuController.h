#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

/**
 ���� ȭ�鿡���� �۵��� ��Ʈ�ѷ� ��������Դϴ�. ���� ���� ������..
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
	
	// ���θ޴����� �������Ʈ ��� ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	class UUserWidget* MainMenuWidgetInstance;

	// ������ ī�޶�� �ʱ� Ʈ������
	UPROPERTY()
	class ACameraActor* MenuCamera;
	
	FTransform InitialCameraTransform;

	// ī�޶� �̵� ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxCameraOffset = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraInterpSpeed = 5.0f;

};
