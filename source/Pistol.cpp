#include "Pistol.h"
#define INT_MAX    2147483647

Pistol::Pistol()
{
	type = PISTOL;
	reloadTime = 2.0f;
	currAmmo = 10;
	magSize = 10;
	ammoCapactity = INT_MAX;
	recoilTime = .33f;
	damage = .34f;
	speed = 320.0f;
	lifeTime = 700.0f;
}


Pistol::~Pistol()
{
}
