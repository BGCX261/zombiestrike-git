#include "Revolver.h"
#include "MovingObject.h"
#include "CreatePistolBullet.h"

Revolver::Revolver(MovingObject* owner)
{
	type = REVOLVER;
	reloadTime = 3.0f;
	currAmmo = 5;
	magSize = 5;
	ammoCapactity = 25;
	recoilTime = 0.5f;
	bulletSpread = 3.0f;
	damage = 50.0f;
	speed = 800.0f;
	lifeTime = 1000.0f;
	m_pOwner = owner;
	owner->AddRef();
}



void Revolver::Fire(float dt)
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

