// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuGameMode.h"
#include "UI/MainMenuController.h"

AMainMenuGameMode::AMainMenuGameMode()
{

	// 게임모드가 사용할 기본 플레이어 컨트롤러를 지정
	PlayerControllerClass = AMainMenuController::StaticClass();
	// ::StaticClass()는 C++ 클래스에 대한 언리얼 엔진의 UClass 정보를 가져오는 역할한다.
	// PlayerControllerClass 변수는 이 UClass 타입의 정보를 받로록 설정되어있다.

}