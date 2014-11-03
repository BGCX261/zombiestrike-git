#include "Revolver.h"
#include "MovingObject.h"
#include "CreatePistolBullet.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"

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
	fire_sound = &GameplayState::GetInstance()->pistol_fire;
}



void Revolver::Fire(float dt)
{
	SGD::AudioManager*	pAudio		= SGD::AudioManager::GetInstance();
	GameplayState*		pGameplay	= GameplayState::GetInstance();

	if (currAmmo > 0)
	{
		//create bullet message
		if (recoilTimer.GetTime() == 0)
		{
			CreatePistolBullet* pMsg = new CreatePistolBullet(this);
			pMsg->QueueMessage();
			pMsg = nullptr;

			if (pAudio->IsAudioPlaying(*fire_sound) == false)
				pAudio->PlayAudio(*fire_sound, false);

			recoilTimer.AddTime(recoilTime);
			currAmmo--;
			if (currAmmo == 0)
			{
				reloadTimer.AddTime(reloadTime);
				reloading = true;
			}
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

