// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Weapon_Widget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
/**
 * 
 */
UCLASS()
class CH3PROJECT_API UWeapon_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Grenade_Count;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Smoke_Count;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Magazine_Size;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Number_Of_Bullets;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponType_Name;

	UPROPERTY(meta = (BindWidget))
	UImage* WeaponType_Image;
	
	
public:
	UFUNCTION(BlueprintCallable)
	void SetTextGrenade_Count(FText GC);
	
	UFUNCTION(BlueprintCallable)
	void SetTextSmoke_Count(FText GC);
	
	UFUNCTION(BlueprintCallable)
	void SetTextMagazine_Size(FText GC);
	
	UFUNCTION(BlueprintCallable)
	void SetTextNumber_of_Bullets(FText GC);
	
	UFUNCTION(BlueprintCallable)
	void SetTextWeaponType_Name(FText GC);

	UFUNCTION(BlueprintCallable)
	void SetTextWeaponType_Image(UTexture2D* Weapon_Image);
};
