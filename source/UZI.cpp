#include "UZI.h"
#include "MovingObject.h"
#include "CreatePistolBullet.h"

UZI::UZI(MovingObject* owner)
{
	type = MAC10;
	reloadTime = 3.0f;
	currAmmo = 20;
	magSize = 20;
	ammoCapactity = 60;
	recoilTime = 0.1f;
	bulletSpread = 6.0f;
	damage = 34.0f;
	speed = 800.0f;
	lifeTime = 1000.0f;
	m_pOwner = owner;
	owner->AddRef();
}

void UZI::Fire(float dt)
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
