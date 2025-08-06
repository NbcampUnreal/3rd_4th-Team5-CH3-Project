// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CH3Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/CH3PlayerController.h"


ACH3Character::ACH3Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}

void ACH3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ACH3PlayerController* MyController = Cast<ACH3PlayerController>(GetController()))
		{
			if (MyController->MoveAction)
			{
				EnhancedInputComponent->BindAction(MyController->MoveAction, ETriggerEvent::Triggered, this, &ACH3Character::Move);
			}
			if (MyController->EquipAction)
			{
				EnhancedInputComponent->BindAction(MyController->EquipAction, ETriggerEvent::Triggered, this, &ACH3Character::Equip);
			}
			if (MyController->FireAction)
			{
				EnhancedInputComponent->BindAction(MyController->FireAction, ETriggerEvent::Triggered, this, &ACH3Character::Fire);
			}
			if (MyController->RollAction)
			{
				EnhancedInputComponent->BindAction(MyController->RollAction, ETriggerEvent::Triggered, this, &ACH3Character::Roll);
			}
		}
	}

}

void ACH3Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// ž�ٿ�� ���� �̱� ������ ī�޶��� ForwardVector�� RightVector�� ����Ͽ� �̵� ������ ����
	AddMovementInput(CameraComp->GetForwardVector(), MovementVector.X);
	AddMovementInput(CameraComp->GetRightVector(), MovementVector.Y);
}

void ACH3Character::Equip(const FInputActionValue& Value)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Equip action triggered!"));
}

void ACH3Character::Fire(const FInputActionValue& Value)
{
	// �߻� �׼��� ó���ϴ� ������ ���⿡ �߰��մϴ�.
	UE_LOG(LogTemp, Warning, TEXT("Fire action triggered!"));
}

void ACH3Character::Roll(const FInputActionValue& Value)
{

	// �� �׼��� ó���ϴ� ������ ���⿡ �߰��մϴ�.
	UE_LOG(LogTemp, Warning, TEXT("Roll action triggered!"));
}