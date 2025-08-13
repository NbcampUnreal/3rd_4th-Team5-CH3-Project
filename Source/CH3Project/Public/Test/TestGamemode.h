// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Spawn/SpawnVolume.h"
#include "TestGamemode.generated.h"

class ASpawnVolume;
class ABaseAICharacter;

USTRUCT(BlueprintType)
struct FWaveConfig // 웨이브
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	int32 EnemyCount = 0; // 적 카운트

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	float SpawnInterval = 1.0f; // 적이 스폰되는 간격(시간)
	
};

UCLASS()
class CH3PROJECT_API ATestGamemode : public AGameModeBase
{
	GENERATED_BODY()


public:
	ATestGamemode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	/** 점수 추가 */
	void AddScore(int32 ScoreAmount);

	/** 게임 오버 처리 */
	void GameOver();

	/** 플레이어 사망 처리 */
	void OnPlayerDeath();


protected:
	
	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<FWaveConfig> Waves; // [ FWaveConfig, FWaveConfig, FWaveConfig ] -> 이건 언리얼 에디터에서 설정한다.

	// 현재 웨이브 -1은 시작 안함!
	UPROPERTY(VisibleAnywhere, Category="Wave")
	int32 CurrentWaveIndex = -1;
	
	// 살아있는 적 수
	UPROPERTY(VisibleAnywhere, Category="Wave")
	int32 AliveEnemies = 0;

	// 스폰해야 할 남은 적
	UPROPERTY(VisibleAnywhere, Category="Wave")
	int32 RemainingToSpawn = 0;
	
	UPROPERTY(EditAnywhere, Category="Wave")
	TSubclassOf<ABaseAICharacter> AIChracter;

	// 스폰 영역
	UPROPERTY()
	TArray<TObjectPtr<ASpawnVolume>> SpawnVolumes; 


	FTimerHandle WaveSpawnTimerHandle;
	FTimerHandle GameTimerHandle;

	void StartNextWave();
	void SpawnOneEnemy();
	void CheckWaveEnd();
	void TickGameTimer();

	bool GetRandomSpawnLocation(FVector& OutLocation) const;


public:
	void StartGame(); // 다른쪽에서 호출 하기 위해 public에 위치

	// 상태 플래그 - 게임이 실행중인지 아닌지
	//bool bGameActive = false;

	// 몬스터가 죽을 때 호출 - MonsterAI에서
	void ReportEnemyDeath();
	
private:
	// 점수/시간 관리
	UPROPERTY(EditAnywhere, Category="Game Rules")
	int32 PlayerScore;

	UPROPERTY(EditAnywhere, Category="Game Rules")
	float TimeLimit = 60.f;

	UPROPERTY(VisibleAnywhere, Category="Game Rules")
	float RemainingTime;

	UPROPERTY(VisibleAnywhere, Category="Game Rules")
	bool bIsGameOver;
	
	// 점수 관련
	UPROPERTY(EditAnywhere, Category="Score")
	int32 ScorePerKill = 100;
};
