#include "FlameThrower.h"
#include "MovingObject.h"
#include "CreateFlameBullet.h"

FlameThrower::FlameThrower(MovingObject* owner) : Listener(this)
{
	type = FLAME_THROWER;
	reloadTime = 4.0f;
	currAmmo = 200;
	magSize = 200;
	ammoCapactity = 500;
	recoilTime = .01f;
	bulletSpread = 5.0f;
	damage = 50.0f;
	speed = 500.0f;
	lifeTime = 700.0f;
	m_pOwner = owner;
	owner->AddRef();
}


FlameThrower::~FlameThrower()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}

void FlameThrower::Fire(float dt)
{
	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0)
		{
			CreateFlameBullet* pMsg = new CreateFlameBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
				reloadTimer.AddTime(reloadTime);
		}

	}
}
