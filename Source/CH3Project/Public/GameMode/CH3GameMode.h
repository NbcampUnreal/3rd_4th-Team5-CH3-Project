#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH3GameMode.generated.h"

class ASpawnVolume;
class ABaseAICharacter;
class ACH3Character;
class UHUD_Widget;

USTRUCT(BlueprintType)
struct FWaveConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 TimeLimitSeconds = 60;
};

UCLASS()
class CH3PROJECT_API ACH3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACH3GameMode();

protected:
	virtual void BeginPlay() override;

	// 웨이브
	UPROPERTY(EditAnywhere, Category = "Wave")
	TArray<FWaveConfig> Waves;

	int32 CurrentWaveIndex = -1;
	int32 AliveEnemies = 0;
	int32 RemainingToSpawn = 0;

	UPROPERTY(EditAnywhere, Category = "Wave")
	TSubclassOf<ABaseAICharacter> MonsterClass;

	UPROPERTY()
	TArray<TObjectPtr<ASpawnVolume>> SpawnVolumes;

	// 게임 시간
	int32 RemainingTimeSeconds = 0;

	// 점수 및 킬
	int32 KillCount = 0;
	int32 Score = 0;

	// 상태
	bool bGameActive = false;

	// 타이머
	FTimerHandle WaveSpawnTimerHandle;
	FTimerHandle GameTimerHandle;

	// HUD
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UHUD_Widget> HUDWidgetClass;

	UPROPERTY()
	UHUD_Widget* HUDWidget;

	// 내부 함수
	void StartNextWave();
	void SpawnOneEnemy();
	void CheckWaveEnd();
	void TickGameTimer();
	void GameOver(bool bVictory);
	bool GetRandomSpawnLocation(FVector& OutLocation) const;

	// HUD 업데이트
	void UpdateHUDTimer();
	void UpdateHUDScore();
	void UpdateHUDKills();

	// 델리게이트 콜백
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnPlayerDestroyed(AActor* DestroyedActor);

public:
	// 외부 호출
	void StartGame();
	void ReportEnemyDeath();
	void ReportPlayerDeath();
};