// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuGameMode.h"
#include "UI/MainMenuController.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	// ���Ӹ�尡 ����� �⺻ �÷��̾� ��Ʈ�ѷ��� ����
	PlayerControllerClass = AMainMenuController::StaticClass();
	// ::StaticClass()�� C++ Ŭ������ ���� �𸮾� ������ UClass ������ �������� �����Ѵ�.
	// PlayerControllerClass ������ �� UClass Ÿ���� ������ �޷η� �����Ǿ��ִ�.
}