#include "Weapon.h"
#include "MovingObject.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"

Weapon::Weapon()
{

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

	recoilTimer.Update(dt);
	if (currAmmo == 0 && pAudio->IsAudioPlaying(*fire_sound) == false)
	{
		if (reloadB == true)
		{
			pAudio->PlayAudio(pGameplay->reload_begin, false);
			reloadB = false;
		}

		if (reloadTimer.Update(dt))
		{
			reloadF = true;
		}

		if (reloadF == true)
		{
			pAudio->PlayAudio(pGameplay->reload_finish, false);
			reloadF = false;
			reloadB = true;
			currAmmo = magSize;
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
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr; 
	}
	m_pOwner = owner;

}
