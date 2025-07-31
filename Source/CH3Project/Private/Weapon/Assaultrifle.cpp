

#include "Weapon/Assaultrifle.h"
#include "Bullet.h"

AAssaultrifle::AAssaultrifle()
{
	SetMaxAmmo(30);
	SetReloadTime(2.0f);
	SetFireMode(EFireMode::FullAuto);
	
};

