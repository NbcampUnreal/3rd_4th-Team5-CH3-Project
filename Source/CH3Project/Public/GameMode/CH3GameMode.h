#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH3GameMode.generated.h"

/**
 * ACH3GameMode
 * - ���� ����, �ð� ����, ���� �ý���, ���� ���� ó�� ���� �ٽ� ���� ����ϴ� Ŭ����
 */
UCLASS()
class CH3PROJECT_API ACH3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// ������
	ACH3GameMode();

	// ���� ���� �� ȣ��
	virtual void BeginPlay() override;

	// �� �����Ӹ��� ȣ��Ǿ� �ð� ���� � ���
	virtual void Tick(float DeltaSeconds) override;

	/** ���� �ð� (�� ����) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timne")
	float TimeLimit = 180.f;

	/** ���� �ð� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float RemainingTime;

	/** �÷��̾��� ���� ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 PlayerScore;

	/** ���� ���� ���θ� ��Ÿ�� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	bool bIsGameOver;

	/** �� óġ �� ������ �߰��ϴ� �Լ� */
	UFUNCTION(BlueprintCallable)
	void AddScore(int32 ScoreAmount);

	/** ���� �ð� �ʰ� �Ǵ� �÷��̾� ��� �� ȣ��Ǵ� ���� ���� ó�� �Լ� */
	UFUNCTION(BlueprintCallable)
	void GameOver();

	/** �÷��̾ ������� �� ȣ���ϴ� �Լ� (�ܺο��� ȣ�� ����) */
	UFUNCTION(BlueprintCallable)
	void OnPlayerDeath();
};