// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SniperRifle.h"

ASniperRifle::ASniperRifle()
{
	SetMaxAmmo(5);
	SetReloadTime(5.0f);
	SetFireMode(EFireMode::SemiAuto);
	
};