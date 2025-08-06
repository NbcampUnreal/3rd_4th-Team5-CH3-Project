#include "GameMode/CH3GameState.h"

// 생성자: 기본값 초기화
ACH3GameState::ACH3GameState()
{
	CurrentScore = 0;
	RemainingTime = 0.f;
	bIsGameOver = false;
}

// 점수 설정
void ACH3GameState::SetScore(int32 NewScore)
{
	CurrentScore = NewScore;
}

// 점수 조회
int32 ACH3GameState::GetScore() const
{
	return CurrentScore;
}

// 남은 시간 설정
void ACH3GameState::SetRemainingTime(float Time)
{
	RemainingTime = Time;
}

// 게임 오버 상태 설정
void ACH3GameState::SetGameOver(bool bOver)
{
	bIsGameOver = bOver;
}