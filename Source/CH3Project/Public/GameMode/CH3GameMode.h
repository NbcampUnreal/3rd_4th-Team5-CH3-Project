#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH3GameMode.generated.h"

/**
 * ACH3GameMode
 * - 게임 진행, 시간 관리, 점수 시스템, 게임 오버 처리 등의 핵심 룰을 담당하는 클래스
 */
UCLASS()
class CH3PROJECT_API ACH3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// 생성자
	ACH3GameMode();

	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 매 프레임마다 호출되어 시간 관리 등에 사용
	virtual void Tick(float DeltaSeconds) override;

	/** 제한 시간 (초 단위) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timne")
	float TimeLimit = 180.f;

	/** 남은 시간 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float RemainingTime;

	/** 플레이어의 현재 점수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 PlayerScore;

	/** 게임 오버 여부를 나타냄 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	bool bIsGameOver;

	/** 적 처치 시 점수를 추가하는 함수 */
	UFUNCTION(BlueprintCallable)
	void AddScore(int32 ScoreAmount);

	/** 제한 시간 초과 또는 플레이어 사망 시 호출되는 게임 오버 처리 함수 */
	UFUNCTION(BlueprintCallable)
	void GameOver();

	/** 플레이어가 사망했을 때 호출하는 함수 (외부에서 호출 가능) */
	UFUNCTION(BlueprintCallable)
	void OnPlayerDeath();
};