// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

ACH3PlayerController::ACH3PlayerController() : DefaultMappingContext(nullptr), MoveAction(nullptr)
{
	bShowMouseCursor = true; // 마우스 커서를 보이게 설정
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
				Subsystem->AddMappingContext(DefaultMappingContext, 0);

			}
		}
	}
}

void ACH3PlayerController::MouseLook() // 마우스 위치를 바라보도록 플레이어 폰을 회전시키는 함수
{
	FHitResult HitResult; // HitResult 구조체를 선언하여 마우스 클릭 위치의 정보를 저장
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult); // 마우스 커서 아래의 충돌 정보를 HitResult에 저장
	if (HitResult.bBlockingHit) // HitResult가 유효한지 확인
	{
		APawn* const PlayerPawn = GetPawn();
		if (PlayerPawn)
		{
			FVector PawnLocation = PlayerPawn->GetActorLocation(); // 현재 플레이어 폰의 위치를 가져옴
			// 타겟 위치를 HitResult의 위치로 설정하되, Z축은 플레이어 폰의 Z축과 동일하게 설정
			FVector TargetLocation = FVector(HitResult.Location.X, HitResult.Location.Y, PawnLocation.Z); 

			if (!PawnLocation.Equals(TargetLocation, 1.0f)) // 플레이어 폰의 위치와 타겟 위치가 다를 때만 회전
			{	
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation); // 타겟 위치를 바라보는 회전값을 계산
				FRotator CurrentRotation = PlayerPawn->GetActorRotation(); // 현재 플레이어 폰의 회전값을 가져옴

				if (!CurrentRotation.Equals(LookAtRotation, 0.1f)) // 현재 회전값과 타겟 회전값이 다를 때만 회전 적용
				{
					PlayerPawn->SetActorRotation(LookAtRotation); // 플레이어 폰의 회전값을 타겟 회전값으로 설정
				}
			}
		}
	}
}

void ACH3PlayerController::PlayerTick(float DeltaTime) // 플레이어 컨트롤러의 틱 함수
{
	Super::PlayerTick(DeltaTime);
	
	MouseLook(); // 마우스 위치를 바라보도록 플레이어 폰을 회전시킴
	
}