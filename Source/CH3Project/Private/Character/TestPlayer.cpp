// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

ATestPlayer::ATestPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestPlayer::Move);

		if (LookAction)
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestPlayer::Look);

		if (FireAction)
		{
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ATestCharacter::FireWeapon);
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &ATestCharacter::StopFireWeapon);
		}
		if (ReloadAction)
		{
			EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATestCharacter::OnReload);
		}
	}
}

void ATestPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		FRotator YawRotation(0, ControlRotation.Yaw, 0);

		// 원래 Forward = X, Right = Y 이지만, 캐릭터가 -90도 회전되어 있으므로
		// Forward <- Y축으로, Right <- -X축으로 보정
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y)* -1.0f;// 앞/뒤
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * -1.0f; // 좌/우

		AddMovementInput(ForwardDirection, MovementVector.Y);  // W/S = 앞/뒤
		AddMovementInput(RightDirection, MovementVector.X);    // A/D = 좌/우
	}
}

void ATestPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

