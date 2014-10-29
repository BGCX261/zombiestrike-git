#include "Pistol.h"
#include "MovingObject.h"
#include "CreatePistolBullet.h"
#define INT_MAX    2147483647

Pistol::Pistol(MovingObject* owner)
{
	type = PISTOL;
	reloadTime = 2.0f;
	currAmmo = 10;
	magSize = 10;
	ammoCapactity = INT_MAX;
	recoilTime = .33f;
	bulletSpread = 5.0f;
	damage = .34f;
	speed = 600.0f;
	lifeTime = 700.0f;
	m_pOwner = owner;
	owner->AddRef();
}


Pistol::~Pistol()
{
	//m_pOwner->Release();
	//m_pOwner = nullptr;
}
void Pistol::Fire(float dt)
{
	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0)
		{
			CreatePistolBullet* pMsg = new CreatePistolBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
				reloadTimer.AddTime(reloadTime);
		}
		
	}
}
