// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Pistol.h"

APistol::APistol()
{
	SetMaxAmmo(15);
	SetReloadTime(1.5f);
	SetFireMode(EFireMode::SemiAuto);
	
};
