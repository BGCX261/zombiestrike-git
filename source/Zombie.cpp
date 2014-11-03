#include "Zombie.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"
#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "AnimationManager.h"
#include "Bullet.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "DestroyObjectMessage.h"
#include "Game.h"
#include "BarbedWire.h"
#include "LandMine.h"
#include "SandBag.h"



Zombie::Zombie() : Listener(this)
{

	damage = 10.0f;
	health = 100.0f;
}


Zombie::~Zombie() 
{
	//SetTarget(nullptr); 
	
}
void Zombie::Update(float dt)
{
	if (isAlive)
	{
		if (currBehavior != nullptr)
			currBehavior->Update(dt, this, m_pTarget->GetPosition());


		// possible turret target
		SGD::Event event = { "ASSESS_THREAT", nullptr, this };
		event.SendEventNow(nullptr);

	}
	else
	{
		DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
		dMsg->QueueMessage();
		dMsg = nullptr;
	}
	MovingObject::Update(dt);
	
}
void Zombie::RetrieveBehavior(std::string name)
{

	if (currBehavior != nullptr)
		currBehavior = nullptr;

	currBehavior = BehaviorManager::GetInstance()->GetBehaviors()[name];
}

/*virtual*/ void Zombie::HandleEvent(const SGD::Event* pEvent)
{
	/*if (pEvent->GetEventID() == "BARBWIRE")
	{
		health -= 10.0f; 
		if (health <= 0)
			isAlive = false;
		
	}*/
}

/*virtual*/ void Zombie::HandleCollision(const IBase* pOther)
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	if (pOther->GetType() == OBJ_BULLET)
	{
		const Bullet* bullet = dynamic_cast<const Bullet*>(pOther);

		health -= bullet->GetDamage();
		if (health <= 0.0f)
		{
			isAlive = false;
		}

		if (pAudio->IsAudioPlaying(GameplayState::GetInstance()->zombie_pain) == false)
			pAudio->PlayAudio(GameplayState::GetInstance()->zombie_pain, false);
	}
	else if (pOther->GetType() == OBJ_BARBEDWIRE)
	{
		const BarbedWire* barbWire = dynamic_cast<const BarbedWire*>(pOther);
		if (barbWire->IsActive())
		{
			health -= 10.0f * Game::GetInstance()->DeltaTime();
			if (health <= 0)
				isAlive = false;
			MovingObject::HandleCollision(pOther);
		}
		


	}
	else if (pOther->GetType() == OBJ_SANDBAG)
	{
		const SandBag* sandbag = dynamic_cast<const SandBag*>(pOther);
		if (sandbag->IsActive())
			MovingObject::HandleCollision(pOther);

	}

	else if (pOther->GetType() == OBJ_LANDMINE)
	{
		const LandMine* landMine = dynamic_cast<const LandMine*>(pOther);
		if (landMine->IsActive())
			isAlive = false;

	}
	

}

void Zombie::SetTarget(BaseObject* target)
{
	if (m_pTarget != nullptr)
	{
		m_pTarget->Release();
		m_pTarget = nullptr;
	}
	m_pTarget = target;
}