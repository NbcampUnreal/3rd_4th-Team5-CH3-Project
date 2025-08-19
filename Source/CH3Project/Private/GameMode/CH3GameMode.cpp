#include "GameMode/CH3GameMode.h"
#include "UI/HUD_Widget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CH3Character.h"
#include "AI/BaseAICharacter.h"
#include "Spawn/SpawnVolume.h"
#include "UI/Weapon_Widget.h"

ACH3GameMode::ACH3GameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACH3GameMode::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 볼륨 찾기
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), Found);
	for (AActor* A : Found)
	{
		if (auto* V = Cast<ASpawnVolume>(A))
		{
			SpawnVolumes.Add(V);
		}
	}

	//// HUD 생성
	//if (HUDWidgetClass)
	//{
	//	HUDWidget = CreateWidget<UHUD_Widget>(GetWorld(), HUDWidgetClass);
	//	if (HUDWidget)
	//	{
	//		HUDWidget->AddToViewport();
	//	}
	//}
	// Weapon HUD 생성 그림판
	if (WeaponWidgetClass)
	{
		HUDWeaponWidget = CreateWidget<UWeapon_Widget>(GetWorld(), WeaponWidgetClass);
		if (HUDWeaponWidget)
		{
			HUDWeaponWidget->AddToViewport();
		}
	}

	// 플레이어 파괴 감시
	if (ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		PlayerChar->OnDestroyed.AddDynamic(this, &ACH3GameMode::OnPlayerDestroyed);
	}

	bGameActive = false;
	CurrentWaveIndex = -1;
	KillCount = 0;
	Score = 0;
}

void ACH3GameMode::StartGame()
{
	if (bGameActive) return;
	bGameActive = true;

	CurrentWaveIndex = -1;
	KillCount = 0;
	Score = 0;

	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().SetTimer(GameTimerHandle, this,
		&ACH3GameMode::TickGameTimer, 1.f, true);

	StartNextWave();
}

void ACH3GameMode::UpdateWeaponMagazine_Size(FString MS)
{
	if (HUDWeaponWidget)
	{
		HUDWeaponWidget->SetTextMagazine_Size(MS);
	}
}

void ACH3GameMode::StartNextWave()
{
	++CurrentWaveIndex;
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		GameOver(true);
		return;
	}

	const FWaveConfig& Wave = Waves[CurrentWaveIndex];
	RemainingToSpawn = Wave.EnemyCount;
	AliveEnemies = 0;
	RemainingTimeSeconds = Wave.TimeLimitSeconds;

	if (Wave.EnemyCount > 0 && MonsterClass)
	{
		GetWorldTimerManager().SetTimer(
			WaveSpawnTimerHandle,
			this,
			&ACH3GameMode::SpawnOneEnemy,
			Wave.SpawnInterval,
			true
		);
	}
}

void ACH3GameMode::SpawnOneEnemy()
{
	if (!bGameActive) return;
	if (RemainingToSpawn <= 0)
	{
		GetWorldTimerManager().ClearTimer(WaveSpawnTimerHandle);
		return;
	}

	FVector SpawnLoc;
	if (GetRandomSpawnLocation(SpawnLoc))
	{
		if (ABaseAICharacter* Spawned = GetWorld()->SpawnActor<ABaseAICharacter>(MonsterClass, SpawnLoc, FRotator::ZeroRotator))
		{
			++AliveEnemies;
			--RemainingToSpawn;

			// 적 사망 감시
			Spawned->OnDestroyed.AddDynamic(this, &ACH3GameMode::OnEnemyDestroyed);
		}
	}
}

void ACH3GameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	ReportEnemyDeath();
}

void ACH3GameMode::OnPlayerDestroyed(AActor* DestroyedActor)
{
	ReportPlayerDeath();
}

void ACH3GameMode::CheckWaveEnd()
{
	if (!bGameActive) return;
	if (AliveEnemies <= 0 && RemainingToSpawn <= 0)
	{
		StartNextWave();
	}
}

void ACH3GameMode::TickGameTimer()
{
	if (!bGameActive) return;

	--RemainingTimeSeconds;
	UpdateHUDTimer();

	if (RemainingTimeSeconds <= 0)
	{
		RemainingTimeSeconds = 0;
		GameOver(false);
	}
}

void ACH3GameMode::GameOver(bool bVictory)
{
	if (!bGameActive) return;
	bGameActive = false;

	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveSpawnTimerHandle);

	RemainingToSpawn = 0;
	AliveEnemies = 0;

	if (bVictory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: VICTORY! Score=%d, Kills=%d"), Score, KillCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: DEFEAT! Score=%d, Kills=%d"), Score, KillCount);
	}
}

bool ACH3GameMode::GetRandomSpawnLocation(FVector& OutLocation) const
{
	if (SpawnVolumes.Num() == 0) return false;

	int32 Index = FMath::RandRange(0, SpawnVolumes.Num() - 1);
	if (ASpawnVolume* Volume = SpawnVolumes[Index])
	{
		OutLocation = Volume->GetRandomPointInVolume();
		return true;
	}
	return false;
}

void ACH3GameMode::ReportEnemyDeath()
{
	if (!bGameActive) return;

	--AliveEnemies;
	++KillCount;
	Score += 100;

	UpdateHUDScore();
	UpdateHUDKills();

	CheckWaveEnd();
}

void ACH3GameMode::ReportPlayerDeath()
{
	if (!bGameActive) return;

	GameOver(false);
}

void ACH3GameMode::UpdateHUDTimer()
{
	if (HUDWidget && HUDWidget->GetWidgetFromName(TEXT("HUD_Timer")))
	{
		if (UTextBlock* TimerText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HUD_Timer"))))
		{
			TimerText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainingTimeSeconds)));
		}
	}
}

void ACH3GameMode::UpdateHUDScore()
{
	if (HUDWidget && HUDWidget->GetWidgetFromName(TEXT("HUD_Score")))
	{
		if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HUD_Score"))))
		{
			ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
		}
	}
}

void ACH3GameMode::UpdateHUDKills()
{
	if (HUDWidget && HUDWidget->GetWidgetFromName(TEXT("HUD_Kills")))
	{
		if (UTextBlock* KillText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HUD_Kills"))))
		{
			KillText->SetText(FText::FromString(FString::Printf(TEXT("Kills: %d"), KillCount)));
		}
	}
}