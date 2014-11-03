#include "Tech9.h"
#include "MovingObject.h"
#include "CreatePistolBullet.h"

Tech9::Tech9(MovingObject* owner)
{
	type = TECH9;
	reloadTime = 2.5f;
	currAmmo = 25;
	magSize = 25;
	ammoCapactity = 100;
	recoilTime = 0.1f;
	bulletSpread = 6.0f;
	damage = 34.0f;
	speed = 800.0f;
	lifeTime = 1000.0f;
	m_pOwner = owner;
	owner->AddRef();
}


Tech9::~Tech9()
{
}

void Tech9::Fire(float dt)
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