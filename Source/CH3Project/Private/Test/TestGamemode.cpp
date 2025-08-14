// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestGamemode.h"
#include "Character/CH3PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Spawn/SpawnVolume.h"
#include "AI/BaseAICharacter.h"
#include "UI/MainMenuController.h"

ATestGamemode::ATestGamemode()
{
	PlayerControllerClass = ACH3PlayerController::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PlayerScore = 0;
	RemainingTime = TimeLimit;
	bIsGameOver = false;
	
}

void ATestGamemode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), Found);
	for (AActor* A : Found)
	{
		if (auto* V = Cast<ASpawnVolume>(A))
		{
			SpawnVolumes.Add(V);
		}
	}

	StartGame();
}


void ATestGamemode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsGameOver) return;

	RemainingTime -= DeltaSeconds;

	if (RemainingTime <= 0.f)
	{
		GameOver();
	}
}

void ATestGamemode::AddScore(int32 ScoreAmount)
{
	if (bIsGameOver) return;

	PlayerScore += ScoreAmount;
	UE_LOG(LogTemp, Log, TEXT("Score Updated: %d"), PlayerScore);
}

void ATestGamemode::GameOver()
{
	if (bIsGameOver) return;

	bIsGameOver = true;
	UE_LOG(LogTemp, Warning, TEXT("Game Over! Final Score: %d"), PlayerScore);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetCinematicMode(true, false, false, true, true);
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
	}

	//UI 추가 시 이곳에서 게임 오버 UI 관련 추가 구현이 필요
}

void ATestGamemode::OnPlayerDeath()
{
	GameOver();
}

void ATestGamemode::StartNextWave()
{
	++CurrentWaveIndex;
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		GameOver();
		return;
	}

	const FWaveConfig& Wave = Waves[CurrentWaveIndex];
	RemainingToSpawn = Wave.EnemyCount;
	AliveEnemies = 0;

	RemainingTime = TimeLimit;


	//위젯 추가 필요 타이머, 웨이브 시작, 스탯 등

	if (Wave.EnemyCount > 0 && AIChracter)
	{
		GetWorldTimerManager().SetTimer(
			WaveSpawnTimerHandle,
			this,
			&ATestGamemode::SpawnOneEnemy,
			Wave.SpawnInterval,
			true);
	}
	
}

void ATestGamemode::SpawnOneEnemy()
{
	if (bIsGameOver) return;

	if (RemainingTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(WaveSpawnTimerHandle);
		return;
	}


	FVector SpawnLoc;
	if (!GetRandomSpawnLocation(SpawnLoc))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn volume available"));
		return;
	}

	FRotator Rot = FRotator::ZeroRotator;
	if (ABaseAICharacter* Spawned = GetWorld()->SpawnActor<ABaseAICharacter>(AIChracter, SpawnLoc, Rot))
	{
		++AliveEnemies;
		--RemainingToSpawn;
	}
}

void ATestGamemode::CheckWaveEnd()
{
	if (bIsGameOver) return;

	if (AliveEnemies <= 0 && RemainingToSpawn <= 0)
	{
		StartNextWave();
	}
}

void ATestGamemode::TickGameTimer()
{
	if (bIsGameOver) return;
	--RemainingTime;

	//위젯 추가할 공간


	if (RemainingTime <= 0)
	{
		RemainingTime = 0;
		GameOver();
	}

}

bool ATestGamemode::GetRandomSpawnLocation(FVector& OutLocation) const
{
	if (SpawnVolumes.Num() == 0)
		return false;

	int32 Index = FMath::RandRange(0, SpawnVolumes.Num() - 1);
	if (ASpawnVolume* Volume = SpawnVolumes[Index])
	{
		OutLocation = Volume->GetRandomPointInVolume();
		return true;
	}

	return false;
}

void ATestGamemode::ReportEnemyDeath()
{
	if (bIsGameOver) return;

	--AliveEnemies;
	AddScore(100);

	/*
	if (위젯명)
	{
		위젯명 -> UpdateStats(Score);
	}

	CheckWaveEnd();
	*/
}


void ATestGamemode::StartGame()
{
	if (!bIsGameOver) return;
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);

		CurrentWaveIndex = -1;
		PlayerScore = 0;

		GetWorldTimerManager().ClearTimer(GameTimerHandle); // 한번 기존 타이머 클리어
		GetWorldTimerManager().SetTimer(GameTimerHandle, this,
										&ATestGamemode::TickGameTimer, 1.f, true);
		StartNextWave();
	}
	
}

