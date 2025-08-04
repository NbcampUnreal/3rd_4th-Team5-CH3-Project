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

public:
	ATestAICharacter();
	
protected:
	virtual void BeginPlay() override; //이 헤더와 cpp파일의 코드는 예시 코드입니다
	

};
