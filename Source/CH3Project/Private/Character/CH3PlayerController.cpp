// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

ACH3PlayerController::ACH3PlayerController() : DefaultMappingContext(nullptr), MoveAction(nullptr)
{
	bShowMouseCursor = true; // ���콺 Ŀ�� ǥ��
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
				Subsystem->AddMappingContext(DefaultMappingContext, 0); // 0�� �켱����, �������� �켱������ ����

			}
		}
	}
}

void ACH3PlayerController::MouseLook()
{
	FHitResult HitResult; // ���콺 Ŀ�� ��ġ���� ��Ʈ ����� ������ ����
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult); // ���콺 Ŀ�� �Ʒ��� ��Ʈ ����� ������
	if (HitResult.bBlockingHit) // ��Ʈ�� �߻����� ��
	{
		APawn* const PlayerPawn = GetPawn(); // ���� �÷��̾��� Pawn�� ������
		if (PlayerPawn)
		{
			// �÷��̾� Pawn�� ��ġ�� ��Ʈ ����� ��ġ�� �̿��Ͽ� �ٶ󺸴� ������ ���
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerPawn->GetActorLocation(), FVector(HitResult.Location.X, HitResult.Location.Y, PlayerPawn->GetActorLocation().Z));
			PlayerPawn->SetActorRotation(LookAtRotation); // �÷��̾� Pawn�� ȸ���� ����
		}
	}
}

void ACH3PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// ���콺 Ŀ�� ��ġ�� ���� �÷��̾ �ٶ󺸴� ������ ����
	MouseLook();
	
}