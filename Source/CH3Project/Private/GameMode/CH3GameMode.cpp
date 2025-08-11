#include "GameMode/CH3GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// ������: �⺻�� �ʱ�ȭ
ACH3GameMode::ACH3GameMode()
{
	PrimaryActorTick.bCanEverTick = true; // Tick �Լ� Ȱ��ȭ
	PlayerScore = 0;
	RemainingTime = TimeLimit;
	bIsGameOver = false;
}

// ���� ���� �� ȣ��Ǵ� �Լ�
void ACH3GameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = TimeLimit;
	bIsGameOver = false;
}

// �� �����Ӹ��� ȣ��Ǿ� ���� �ð� üũ
void ACH3GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ������ ���� ��� ó������ ����
	if (bIsGameOver) return;

	RemainingTime -= DeltaSeconds;

	// ���� �ð� ���� �� ���� ���� ó��
	if (RemainingTime <= 0.f)
	{
		GameOver();
	}
}

// ������ �߰��ϴ� �Լ�
void ACH3GameMode::AddScore(int32 ScoreAmount)
{
	if (bIsGameOver) return;

	PlayerScore += ScoreAmount;

	// �α׷� ���� ���
	UE_LOG(LogTemp, Log, TEXT("Score Updated: %d"), PlayerScore);
}

// ���� ���� ó�� �Լ�
void ACH3GameMode::GameOver()
{
	if (bIsGameOver) return;

	bIsGameOver = true;

	UE_LOG(LogTemp, Warning, TEXT("Game Over! Final Score: %d"), PlayerScore);

	// �÷��̾� �Է� ���� �� ���콺 Ŀ�� Ȱ��ȭ
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetCinematicMode(true, false, false, true, true); // �Է� ����
		PC->bShowMouseCursor = true;
		PC->SetPause(true); // ���� �Ͻ�����
	}
}

// �ܺο��� ȣ���ϴ� �÷��̾� ��� �� ó�� �Լ�
void ACH3GameMode::OnPlayerDeath()
{
	GameOver(); // ������ GameOver ���� ����
}