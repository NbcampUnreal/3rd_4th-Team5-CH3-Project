// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Grenade,
	Consumable
};


UCLASS()
class CH3PROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UStaticMeshComponent* ItemMesh;


	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI") //이 구간은 추후 인벤토리 시스템과 연동 가능하도록 구현 중
	UTexture2D* Icon;
	*/

	//virtual void Use(class ATestCharacter* Character); //추후 수정 필요
	
protected:
	virtual void BeginPlay() override; 

public:	
	virtual void Tick(float DeltaTime) override;

};
