//#include "CH3GameMode.h"
//#include "CH3GameState.h"
//#include "Kismet/GameplayStatics.h"
//#include "TimerManager.h"
//
//ACH3GameMode::ACH3GameMode()
//{
//    GameStateClass = ACH3GameState::StaticClass();
//}
//
//void ACH3GameMode::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // 1초마다 TickCountdown 실행
//    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACH3GameMode::TickTime, 1.0f, true);
//}
//
//void ACH3GameMode::TickTime()
//{
//    ACH3GameState* GS = GetGameState<ACH3GameState>();
//    if (GS && !GS->bIsGameOver)
//    {
//        GS->RemainingTime -= 1.0f;
//
//        if (GS->RemainingTime <= 0.0f)
//        {
//            GameOver();
//        }
//    }
//}
//
//void ACH3GameMode::AddScore(int32 Amount)
//{
//    ACH3GameState* GS = GetGameState<ACH3GameState>();
//    if (GS)
//    {
//        GS->AddScore(Amount);
//    }
//}
//
//void ACH3GameMode::GameOver()
//{
//    ACH3GameState* GS = GetGameState<ACH3GameState>();
//    if (GS && !GS->bIsGameOver)
//    {
//        GS->SetGameOver();
//
//        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
//
//        // 추가 처리: 입력 차단, UI 띄우기 등
//    }
//}