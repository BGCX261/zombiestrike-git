#include "PukerBlaster.h"
#include "MovingObject.h"
#include "CreatePukeBullet.h"
#include "GameplayState.h"

PukerBlaster::PukerBlaster(MovingObject* owner)
{
	type = FTHROWER;
	reloadTime = 5.0f;
	currAmmo = INT_MAX;
	magSize = 100;
	ammoCapactity = 500;
	recoilTime = .02f;
	bulletSpread = 3.0f;
	damage = 2.0f;
	speed = 500.0f;
	lifeTime = 700.0f;
	m_pOwner = owner;
	owner->AddRef();
	fire_sound = &GameplayState::GetInstance()->vomit_fire;
}


PukerBlaster::~PukerBlaster()
{
}

void PukerBlaster::Fire(float dt)
{
	SGD::AudioManager*	pAudio = SGD::AudioManager::GetInstance();

	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0)
		{
			CreatePukeBullet* pMsg = new CreatePukeBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			if (pAudio->IsAudioPlaying(*fire_sound) == false)
				pAudio->PlayAudio(*fire_sound, false);

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
				reloadTimer.AddTime(reloadTime);
		}

	}
}
