#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CH3GameState.generated.h"

/**
 * ACH3GameState
 * - ������ ����, ���� �ð�, ���� ���� ���� ���� ���� �� �����ϴ� Ŭ����
 * - �ַ� UI���� ���� �Ǵ� ��Ʈ��ũ ����ȭ�� ���
 */
UCLASS()
class CH3PROJECT_API ACH3GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACH3GameState();

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 CurrentScore;

	// ���� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float RemainingTime;

	// ���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	bool bIsGameOver;

	// ���� ���� �Լ�
	void SetScore(int32 NewScore);

	// ���� �������� �Լ�
	int32 GetScore() const;

	// ���� �ð� ���� �Լ�
	void SetRemainingTime(float Time);

	// ���� ���� ���� ���� �Լ�
	void SetGameOver(bool bOver);
};