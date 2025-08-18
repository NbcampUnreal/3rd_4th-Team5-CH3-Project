// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character_Stats_Widget.h"
#include "Components/TextBlock.h"

void UCharacter_Stats_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	// 필요 시 기본값 지정
	if (ProgressBar_HP_Bar)
	{
		ProgressBar_HP_Bar->SetPercent(1.0f); // 시작 체력 100%
	}
	if (ProgressBar_ST_Bar)
	{
		ProgressBar_ST_Bar->SetPercent(1.0f); // 시작 스태미나/대시 준비 100%
	}
}

void UCharacter_Stats_Widget::SetProgressHPBar(int HP)
{
	if (!ProgressBar_HP_Bar) return;

	// 정수 입력(0~100 가정)을 0~1로 변환
	const float Percent = FMath::Clamp(static_cast<float>(HP) / 100.f, 0.f, 1.f);
	ProgressBar_HP_Bar->SetPercent(Percent);
}

void UCharacter_Stats_Widget::SetProgressSTBar(int ST)
{
	if (!ProgressBar_ST_Bar) return;

	// 정수 입력(0~100 가정)을 0~1로 변환
	const float Percent = FMath::Clamp(static_cast<float>(ST) / 100.f, 0.f, 1.f);
	ProgressBar_ST_Bar->SetPercent(Percent);
}

void UCharacter_Stats_Widget::SetProgressHPPercent(float HPPercent)
{
	if (!ProgressBar_HP_Bar) return;

	ProgressBar_HP_Bar->SetPercent(FMath::Clamp(HPPercent, 0.f, 1.f));
}

void UCharacter_Stats_Widget::SetProgressSTPercent(float STPercent)
{
	if (!ProgressBar_ST_Bar) return;

	ProgressBar_ST_Bar->SetPercent(FMath::Clamp(STPercent, 0.f, 1.f));
}