#include "Weapon.h"
#include "MovingObject.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"

Weapon::Weapon()
{
	reloadB = true;
	reloadF = false;
}


Weapon::~Weapon()
{
	SetOwner(nullptr);
	fire_sound = nullptr;
}
void Weapon::Update(float dt)
{
	SGD::AudioManager*	pAudio		= SGD::AudioManager::GetInstance();
	GameplayState*		pGameplay	= GameplayState::GetInstance();

	/*
	recoilTimer.Update(dt);
	if (currAmmo == 0 && pAudio->IsAudioPlaying(*fire_sound) == false && totalAmmo > 0)
	{
		if (reloadB == true)
		{
			pAudio->PlayAudio(pGameplay->reload_begin, false);
			reloadB = false;
		}

		if (reloadTimer.Update(dt))
		{
			reloadF = true;

			if (totalAmmo >= magSize)
			{
				currAmmo = magSize;
				totalAmmo -= magSize;
			}
			else
			{
				currAmmo = totalAmmo;
				totalAmmo = 0;
			}
		}

		if (reloadF == true)
		{
			pAudio->PlayAudio(pGameplay->reload_finish, false);
			reloadF = false;
			reloadB = true;
		}
	}
	*/

	recoilTimer.Update(dt);
	if (reloading == true && (pAudio->IsVoiceValid(voice) == false || pAudio->IsVoicePlaying(voice) == false) && totalAmmo > 0)
	{
		// unload empty magazine
		if (reloadB == true)
		{
			pAudio->PlayAudio(pGameplay->reload_begin, false);
			reloadB = false;
		}

		// reload finished
		if (reloadTimer.Update(dt))
		{
			reloadF = true;

			// all ammo gone
			if (currAmmo == 0)
			{
				if (totalAmmo >= magSize)
				{
					currAmmo = magSize;
					totalAmmo -= magSize;
				}
				else
				{
					currAmmo = totalAmmo;
					totalAmmo = 0;
				}
			}

			// some ammo gone
			else if (currAmmo > 0)
			{
				int bullets_needed = magSize - currAmmo;

				if (totalAmmo >= bullets_needed)
				{
					totalAmmo -= bullets_needed;
					currAmmo += bullets_needed;
				}
			}
		}

		// load full magazine
		if (reloadF == true)
		{
			pAudio->PlayAudio(pGameplay->reload_finish, false);
			reloadF = false;
			reloadB = true;
			reloading = false;
		}
	}

}

void Weapon::Fire(float dt)
{
	//if (currAmmo > 0)
	//{
	//	//create bullet message
	//	CreateBulletMessage* pMsg = new CreateBulletMessage(m_pOwner);
	//	pMsg->QueueMessage();
	//	pMsg = nullptr;

	//	recoilTimer.AddTime(recoilTime);
	//	currAmmo--; 
	//	if (currAmmo == 0)
	//		reloadTimer.AddTime(reloadTime);
	//}

}
void Weapon::SetOwner(MovingObject* owner)
{
	if (owner != m_pOwner)
	{
		if (m_pOwner != nullptr)
		{
			m_pOwner->Release();
			m_pOwner = nullptr;
		}
		m_pOwner = owner;
		if (m_pOwner != nullptr)
			m_pOwner->AddRef();
	}
	

}

void Weapon::ReloadEntire(void)
{
	currAmmo = 0;
	reloadTimer.AddTime(reloadTime);
	reloading = true;
}

void Weapon::ReloadNeeded(void)
{
	if (currAmmo >= magSize)
		return;

	int bullets_needed = magSize - currAmmo;

	if (totalAmmo >= bullets_needed)
	{
		reloadTimer.AddTime(reloadTime);
		reloading = true;
	}
}
