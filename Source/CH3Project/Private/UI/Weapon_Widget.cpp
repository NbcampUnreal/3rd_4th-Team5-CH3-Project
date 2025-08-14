// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapon_Widget.h"
#include "Components/TextBlock.h"
// 수류탄 소지갯수
void UWeapon_Widget::SetTextGrenade_Count(FText GC)
{
	Grenade_Count->SetText(GC);
}
// 섬광탄 소지 갯수
void UWeapon_Widget::SetTextSmoke_Count(FText SC)
{
	Smoke_Count->SetText(SC);
}
// 장전된 탄약
void UWeapon_Widget::SetTextMagazine_Size(FText MS)
{
	Magazine_Size->SetText(MS);
}
// 탄약 총 소지 갯수
void UWeapon_Widget::SetTextNumber_of_Bullets(FText NoB)
{
	Number_Of_Bullets->SetText(NoB);
}
// 
void UWeapon_Widget::SetTextWeaponType_Name(FText WTN)
{
	WeaponType_Name->SetText(WTN);
}

void UWeapon_Widget::SetTextWeaponType_Image(UTexture2D* Weapon_Image)
{
	// if (UTexture2D* Texture = Cast<UTexture2D>(Weapon_Image))
	// {} UObject나 AActor로 받아서 cast로 검증단계를 ㅁ만들어보자 (시간날때)
		WeaponType_Image->SetBrushFromTexture(Weapon_Image);
}


