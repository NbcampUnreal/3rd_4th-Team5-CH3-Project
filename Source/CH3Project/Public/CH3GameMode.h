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
//    // ���� ����
//    UFUNCTION(BlueprintCallable)
//    void StartGame();
//
//    // ���� ����
//    UFUNCTION(BlueprintCallable)
//    void EndGame(bool bPlayerDead);
//
//    // ���� �߰�
//    UFUNCTION(BlueprintCallable)
//    void AddScore(int32 Points);
//
//protected:
//    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
//    float GameDuration = 180.f; // 3�� ���� �ð�
//
//    FTimerHandle TimerHandle_GameOver;
//
//private:
//    int32 PlayerScore;
//};
