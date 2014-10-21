#include "AssaultRifle.h"


AssaultRifle::AssaultRifle()
{
	type = ASSUALT_RIFLE;
	reloadTime = 2.5f;
	currAmmo = 25;
	magSize = 25;
	ammoCapactity = 100;
	recoilTime = 0.2f;
	damage = 34.0f;
	speed = 600.0f;
	lifeTime = 1000.0f;

}


AssaultRifle::~AssaultRifle()
{
}
