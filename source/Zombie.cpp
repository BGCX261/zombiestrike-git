#include "Zombie.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "Player.h"


Zombie::Zombie() : Listener(this)
{
}

Zombie::~Zombie()
{
}

/*virtual*/ void Zombie::Update(float dt)	 /*override*/
{
	if (m_tStunTimer.Update(dt))
	{
		if (currBehavior != nullptr)
		{
			currBehavior->Update(dt, this, m_pTarget->GetPosition());
		}
	}

	MovingObject::Update(dt);
}

/*virtual*/ void Zombie::HandleEvent(const SGD::Event* pEvent)
{
}

/*virtual*/ void Zombie::HandleCollision(const IBase* pOther) /*override*/
{
	if (pOther->GetType() == ObjectType::OBJ_PLAYER)
	{
		SGD::Event hitPlayer = { "HIT", nullptr, this };
		hitPlayer.SendEventNow(m_pTarget);
	}
	else if (pOther->GetType() == ObjectType::OBJ_TRIGGER || pOther->GetType() == ObjectType::OBJ_PLAYERCOLLISION)
	{
	}
	else
	{
		MovingObject::HandleCollision(pOther);
	}
}

void Zombie::SetTarget(Player* _target)
{
	if (m_pTarget != nullptr)
	{
		m_pTarget->Release();
		m_pTarget = nullptr;
	}
	m_pTarget = _target;
}

void Zombie::RetrieveBehavior(std::string name)
{

	if (currBehavior != nullptr)
	{
		currBehavior = nullptr;
	}
	currBehavior = BehaviorManager::GetInstance()->GetBehaviors()[name];
}

