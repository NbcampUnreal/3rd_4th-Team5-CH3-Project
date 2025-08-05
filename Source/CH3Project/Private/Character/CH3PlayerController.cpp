// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

ACH3PlayerController::ACH3PlayerController() : DefaultMappingContext(nullptr), MoveAction(nullptr)
{
	bShowMouseCursor = true; // 마우스 커서 표시
}

void ACH3PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0); // 0은 우선순위, 낮을수록 우선순위가 높음

			}
		}
	}
}

void ACH3PlayerController::MouseLook()
{
	FHitResult HitResult; // 마우스 커서 위치에서 히트 결과를 저장할 변수
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult); // 마우스 커서 아래의 히트 결과를 가져옴
	if (HitResult.bBlockingHit) // 히트가 발생했을 때
	{
		APawn* const PlayerPawn = GetPawn(); // 현재 플레이어의 Pawn을 가져옴
		if (PlayerPawn)
		{
			// 플레이어 Pawn의 위치와 히트 결과의 위치를 이용하여 바라보는 방향을 계산
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerPawn->GetActorLocation(), FVector(HitResult.Location.X, HitResult.Location.Y, PlayerPawn->GetActorLocation().Z));
			PlayerPawn->SetActorRotation(LookAtRotation); // 플레이어 Pawn의 회전을 설정
		}
	}
}

void ACH3PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// 마우스 커서 위치에 따라 플레이어가 바라보는 방향을 조정
	MouseLook();
	
}