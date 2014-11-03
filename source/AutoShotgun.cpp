#include "AutoShotgun.h"
#include "MovingObject.h"
#include "CreateShotgunBullet.h"

AutoShotgun::AutoShotgun(MovingObject* owner)
{
	type = SHOTGUN;
	reloadTime = 4.0f;
	currAmmo = 6;
	magSize = 6;
	ammoCapactity = 24;
	recoilTime = 1.0f;
	bulletSpread = 15.0f;
	damage = 50.0f;
	speed = 700.0f;
	lifeTime = 400.0f;
	m_pOwner = owner;
	owner->AddRef();
}


AutoShotgun::~AutoShotgun()
{
}

void AutoShotgun::Fire(float dt)
{
	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0)
		{
			CreateShotgunBullet* pMsg = new CreateShotgunBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
				reloadTimer.AddTime(reloadTime);
		}

	}
}
