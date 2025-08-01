// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestCharacter.h"
#include "TestAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class CH3PROJECT_API ATestAICharacter : public ATestCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override; //이 헤더와 cpp파일의 코드는 예시 코드입니다
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") //블루 프린트에서 Weapon 카테고리에서 Weapon을 설정할 수 있도록 반영했습니다.
	TSubclassOf<AWeapon> WeaponClass;
	
};
