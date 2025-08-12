#include "GameMode/CH3GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


// 생성자: 기본값 초기화
ACH3GameMode::ACH3GameMode()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 함수 활성화


	PlayerScore = 0;
	RemainingTime = TimeLimit;
	bIsGameOver = false;
}


// 게임 시작 시 호출되는 함수
void ACH3GameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = TimeLimit;
	bIsGameOver = false;
}


// 매 프레임마다 호출되어 남은 시간 체크
void ACH3GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	// 게임이 끝난 경우 처리하지 않음
	if (bIsGameOver) return;

	RemainingTime -= DeltaSeconds;


	// 제한 시간 종료 시 게임 오버 처리
	if (RemainingTime <= 0.f)
	{
		GameOver();
	}
}


// 점수를 추가하는 함수
void ACH3GameMode::AddScore(int32 ScoreAmount)
{
	if (bIsGameOver) return;

	PlayerScore += ScoreAmount;


	// 로그로 점수 출력
	UE_LOG(LogTemp, Log, TEXT("Score Updated: %d"), PlayerScore);
}

// 게임 오버 처리 함수

void ACH3GameMode::GameOver()
{
	if (bIsGameOver) return;

	bIsGameOver = true;

	UE_LOG(LogTemp, Warning, TEXT("Game Over! Final Score: %d"), PlayerScore);


	// 플레이어 입력 차단 및 마우스 커서 활성화
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetCinematicMode(true, false, false, true, true); // 입력 차단
		PC->bShowMouseCursor = true;
		PC->SetPause(true); // 게임 일시정지
	}
}

// 외부에서 호출하는 플레이어 사망 시 처리 함수
void ACH3GameMode::OnPlayerDeath()
{
	GameOver(); // 동일한 GameOver 로직 실행
	
}