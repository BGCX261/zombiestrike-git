#include "Sniper.h"
#include "MovingObject.h"


Sniper::Sniper(MovingObject* owner)
{
	type = SNIPER;
	reloadTime = 3.5f;
	currAmmo = 5;
	magSize = 5;
	ammoCapactity = 10;
	recoilTime = .5f;
	damage = 100.0f;
	speed = 750.0f;
	lifeTime = 1000.0f;
	m_pOwner = owner;
	owner->AddRef();
}


Sniper::~Sniper()
{
	//m_pOwner->Release();
	//m_pOwner = nullptr;
}
