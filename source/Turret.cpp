#include "Turret.h"
#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "GameplayState.h"
#include "CreateBulletMessage.h"
#include "BaseObject.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


Turret::Turret() : Listener(this)
{
	RegisterForEvent("TARGET_FOUND");
	RegisterForEvent("LOST_TARGET");
	RegisterForEvent("UNPAUSE");
	RegisterForEvent("PAUSE");

	fireSound	= &GameplayState::GetInstance()->turretfire;

	this->SetAnimation("turret");
}

Turret::~Turret()
{
	UnregisterFromEvent("TARGET_FOUND");
	UnregisterFromEvent("LOST_TARGET");
	UnregisterFromEvent("UNPAUSE");
	UnregisterFromEvent("PAUSE");
}

/*virtual*/ void Turret::Update(float dt)	 /*override*/
{
	if (currBehavior != nullptr)
	{
		if (isActive == true)
		{
			if (currBehavior->Update(dt, this, this->m_ptPosition))
			{
				// play sound
				

				// fire bulet

			}
		}
	}

	MovingObject::Update(dt);
}

/*virtual*/ void Turret::HandleEvent(const SGD::Event* pEvent)
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	if (pEvent->GetEventID() == "LOST_TARGET")
	{
		isActive = false;
	}
	else if (pEvent->GetEventID() == "TARGET_FOUND")
	{
		isActive = true;
	}
	else if (pEvent->GetEventID() == "PAUSE")
	{
		if (pAudio->IsAudioPlaying(*fireSound) == true)
			pAudio->StopAudio(*fireSound);
	}
	else if (pEvent->GetEventID() == "UNPAUSE")
	{
		if (isActive == true)
			pAudio->PlayAudio(*fireSound, false);
	}
}

/*virtual*/ void Turret::HandleCollision(const IBase* pOther) /*override*/
{

}

void Turret::RetrieveBehavior(std::string name)
{
	if (currBehavior != nullptr)
	{
		currBehavior = nullptr;
	}
	currBehavior = BehaviorManager::GetInstance()->GetBehaviors()[name];
}

/*virtual*/ void Turret::Attack() /*override*/
{
	m_tShotTimer.AddTime(0.1f);
	CreateBulletMessage* pMsg = new CreateBulletMessage(this);
	pMsg->QueueMessage();
	pMsg = nullptr;
}
