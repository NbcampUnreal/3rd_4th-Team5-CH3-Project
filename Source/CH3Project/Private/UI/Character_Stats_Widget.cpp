// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character_Stats_Widget.h"
#include "Components/TextBlock.h"

/*
void UHUD_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ProgressBar_HP_Bar && ProgressBar_ST_Bar)
	{
		ProgressBar_HP_Bar->SetPercent(0.1);
		ProgressBar_ST_Bar->SetPercent(0.1);
	}
}
*/

void UCharacter_Stats_Widget::SetProgressHPBar(int HP)
{
	ProgressBar_HP_Bar->SetPercent(HP);
}

void UCharacter_Stats_Widget::SetProgressSTBar(int ST)
{
	ProgressBar_ST_Bar->SetPercent(ST);
}