#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CH3GameState.generated.h"

/**
 * ACH3GameState
 * - 게임의 점수, 남은 시간, 게임 오버 상태 등을 저장 및 공유하는 클래스
 * - 주로 UI와의 연동 또는 네트워크 동기화에 사용
 */
UCLASS()
class CH3PROJECT_API ACH3GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACH3GameState();

	// 현재 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 CurrentScore;

	// 남은 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float RemainingTime;

	// 게임 오버 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	bool bIsGameOver;

	// 점수 설정 함수
	void SetScore(int32 NewScore);

	// 점수 가져오기 함수
	int32 GetScore() const;

	// 남은 시간 설정 함수
	void SetRemainingTime(float Time);

	// 게임 오버 상태 설정 함수
	void SetGameOver(bool bOver);
};