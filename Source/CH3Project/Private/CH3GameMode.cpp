//#include "CH3GameMode.h"
//#include "Kismet/GameplayStatics.h"
//#include "GameFramework/PlayerController.h"
//#include "TimerManager.h"
//
//ACH3GameMode::ACH3GameMode()
//{
//    PlayerScore = 0;
//}
//
//void ACH3GameMode::BeginPlay()
//{
//    Super::BeginPlay();
//    StartGame();
//}
//
//void ACH3GameMode::StartGame()
//{
//    // ���� �ð� Ÿ�̸� ����
//    GetWorldTimerManager().SetTimer(TimerHandle_GameOver, this, &ACH3GameMode::EndGame, GameDuration, false);
//}
//
//void ACH3GameMode::EndGame(bool bPlayerDead)
//{
//    FString EndReason = bPlayerDead ? TEXT("�÷��̾� ���") : TEXT("���� �ð� �ʰ�");
//    UE_LOG(LogTemp, Warning, TEXT("���� ����: %s"), *EndReason);
//
//    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
//    if (PC)
//    {
//        PC->SetPause(true);
//    }
//
//    // ��� ȭ�� ��� or ���� �޴� ��ȯ �� ����
//}
//
//void ACH3GameMode::AddScore(int32 Points)
//{
//    PlayerScore += Points;
//    UE_LOG(LogTemp, Log, TEXT("���� ����: %d"), PlayerScore);
//}