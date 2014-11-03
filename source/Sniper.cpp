#include "Sniper.h"
#include "MovingObject.h"
#include "CreateSniperBullet.h"

Sniper::Sniper(MovingObject* owner)
{
	type = SPECIAL;
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

void Sniper::Fire(float dt)
{
	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0)
		{
			CreateSniperBullet* pMsg = new CreateSniperBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
				reloadTimer.AddTime(reloadTime);
		}

	}
}