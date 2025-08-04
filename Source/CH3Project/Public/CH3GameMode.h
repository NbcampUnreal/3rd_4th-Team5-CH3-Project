//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/GameMode.h"
//#include "CH3GameMode.generated.h"
//
//UCLASS()
//class CH3PROJECT_API ACH3GameMode : public AGameMode
//{
//	GENERATED_BODY()
//	
//public:
//    ACH3GameMode();
//
//    virtual void BeginPlay() override;
//
//    // 게임 시작
//    UFUNCTION(BlueprintCallable)
//    void StartGame();
//
//    // 게임 종료
//    UFUNCTION(BlueprintCallable)
//    void EndGame(bool bPlayerDead);
//
//    // 점수 추가
//    UFUNCTION(BlueprintCallable)
//    void AddScore(int32 Points);
//
//protected:
//    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
//    float GameDuration = 180.f; // 3분 제한 시간
//
//    FTimerHandle TimerHandle_GameOver;
//
//private:
//    int32 PlayerScore;
//};
