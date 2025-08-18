// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapon_Widget.h"
#include "Components/TextBlock.h"

void UWeapon_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	WeaponType_Image->SetVisibility(ESlateVisibility::Visible);
}

// 수류탄 소지갯수
void UWeapon_Widget::SetTextGrenade_Count(FString GC)
{
	Grenade_Count->SetText(FText::FromString(GC));
}
// 섬광탄 소지 갯수
void UWeapon_Widget::SetTextSmoke_Count(FString SC)
{
		Smoke_Count->SetText(FText::FromString(SC));
}
// 장전된 탄약
void UWeapon_Widget::SetTextMagazine_Size(FString MS)
{
	if (Magazine_Size)
	{
		Magazine_Size->SetText(FText::FromString(MS));
		UE_LOG(LogTemp, Warning, TEXT("정상"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("문제있나봄"));
	}
}
// 탄약 총 소지 갯수
void UWeapon_Widget::SetTextNumber_of_Bullets(FString NoB)
{
	Number_Of_Bullets->SetText(FText::FromString(NoB));
}
// 
void UWeapon_Widget::SetTextWeaponType_Name(FString WTI)
{
	WeaponType_Name->SetText(FText::FromString(WTI));
}

void UWeapon_Widget::SetTextWeaponType_Image(UTexture2D* WTI)
{
	// if (UTexture2D* Texture = Cast<UTexture2D>(Weapon_Image))
	// {} UObject나 AActor로 받아서 cast로 검증단계를 ㅁ만들어보자 (시간날때)
		WeaponType_Image->SetBrushFromTexture(WTI);
}


