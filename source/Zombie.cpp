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
#include "SpawnManager.h"



Zombie::Zombie() : Listener(this)
{


}


Zombie::~Zombie() 
{
	
	
}
void Zombie::Update(float dt)
{
	if (isAlive)
	{
		if (currBehavior != nullptr)
			currBehavior->Update(dt, this, m_pTarget->GetPosition());
	}
	else
	{
		DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
		dMsg->QueueMessage();
		dMsg = nullptr;

		SpawnManager::GetInstance()->SetEnemiesKilled(SpawnManager::GetInstance()->GetEnemiesKilled() + 1);
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
	if (pOther->GetType() == OBJ_BULLET)
	{
		const Bullet* bullet = dynamic_cast<const Bullet*>(pOther);

		health -= bullet->GetDamage();
		if (health <= 0.0f)
		{
			isAlive = false;
			isCounted = true;

			if (isCounted == true)
			{
				

				isCounted = false;
			}
			
		}
	}
	else if (pOther->GetType() == OBJ_BARBEDWIRE)
	{

		health -= 10.0f * Game::GetInstance()->DeltaTime();
		if (health <= 0)
		{
			isAlive = false;

			//SpawnManager::GetInstance()->SetEnemiesKilled(SpawnManager::GetInstance()->GetEnemiesKilled() + 1);
		}

	}
	else if (pOther->GetType() == OBJ_SANDBAG)
		MovingObject::HandleCollision(pOther);

	else if (pOther->GetType() == OBJ_LANDMINE)
	{
		isAlive = false;

		//SpawnManager::GetInstance()->SetEnemiesKilled(SpawnManager::GetInstance()->GetEnemiesKilled() + 1);
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