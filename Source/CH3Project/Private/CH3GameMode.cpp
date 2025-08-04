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
//    // 제한 시간 타이머 시작
//    GetWorldTimerManager().SetTimer(TimerHandle_GameOver, this, &ACH3GameMode::EndGame, GameDuration, false);
//}
//
//void ACH3GameMode::EndGame(bool bPlayerDead)
//{
//    FString EndReason = bPlayerDead ? TEXT("플레이어 사망") : TEXT("제한 시간 초과");
//    UE_LOG(LogTemp, Warning, TEXT("게임 종료: %s"), *EndReason);
//
//    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
//    if (PC)
//    {
//        PC->SetPause(true);
//    }
//
//    // 결과 화면 출력 or 메인 메뉴 전환 등 가능
//}
//
//void ACH3GameMode::AddScore(int32 Points)
//{
//    PlayerScore += Points;
//    UE_LOG(LogTemp, Log, TEXT("현재 점수: %d"), PlayerScore);
//}