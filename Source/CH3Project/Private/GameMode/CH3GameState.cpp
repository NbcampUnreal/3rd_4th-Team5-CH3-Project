#include "GameMode/CH3GameState.h"

// ������: �⺻�� �ʱ�ȭ
ACH3GameState::ACH3GameState()
{
	CurrentScore = 0;
	RemainingTime = 0.f;
	bIsGameOver = false;
}

// ���� ����
void ACH3GameState::SetScore(int32 NewScore)
{
	CurrentScore = NewScore;
}

// ���� ��ȸ
int32 ACH3GameState::GetScore() const
{
	return CurrentScore;
}

// ���� �ð� ����
void ACH3GameState::SetRemainingTime(float Time)
{
	RemainingTime = Time;
}

// ���� ���� ���� ����
void ACH3GameState::SetGameOver(bool bOver)
{
	bIsGameOver = bOver;
}