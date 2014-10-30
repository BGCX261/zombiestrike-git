#include "AssaultRifle.h"
#include "MovingObject.h"
#include "CreatePistolBullet.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"

AssaultRifle::AssaultRifle(MovingObject* owner)
{
	
	type = ASSUALT_RIFLE;
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
	fire_sound = &GameplayState::GetInstance()->rifle_fire;
}


AssaultRifle::~AssaultRifle()
{
	/*m_pOwner->Release();
	m_pOwner = nullptr;*/
}

void AssaultRifle::Fire(float dt)
{
	SGD::AudioManager*	pAudio		= SGD::AudioManager::GetInstance();
	GameplayState*		pGameplay	= GameplayState::GetInstance();

	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0 && pAudio->IsAudioPlaying(pGameplay->reload_finish) == false)
		{
			CreatePistolBullet* pMsg = new CreatePistolBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			//if (pAudio->IsAudioPlaying(*fire_sound) == false)
			pAudio->PlayAudio(*fire_sound, false);

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
				reloadTimer.AddTime(reloadTime);
		}
	}
	else
	{
		if (pAudio->IsAudioPlaying(pGameplay->out_of_ammo) == false && pAudio->IsAudioPlaying(*fire_sound) == false
			&& pAudio->IsAudioPlaying(pGameplay->reload_begin) == false
			&& pAudio->IsAudioPlaying(pGameplay->reload_finish) == false)
			pAudio->PlayAudio(pGameplay->out_of_ammo, false);
	}
}
