#include "Shotgun.h"


Shotgun::Shotgun()
{

	type = SHOTGUN;
	reloadTime = 4.0f;
	currAmmo = 6;
	magSize = 6;
	ammoCapactity = 24;
	recoilTime = 1.0f;
	damage = 50.0f;
	speed = 200.0f;
	lifeTime = 400.0f;
}


Shotgun::~Shotgun()
{
}
