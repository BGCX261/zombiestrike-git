#include "Bullet.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "BaseObject.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "AnimationManager.h"
#include "DestroyObjectMessage.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Bullet::~Bullet()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}

/*virtual*/ void Bullet::Update(float elapsedTime) /*override*/
{
	MovingObject::Update(elapsedTime);

	if (IsDead())
	{
		DestroyObjectMessage* pMsg = new DestroyObjectMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

/*virtual*/ void Bullet::HandleCollision(const IBase* pOther)	/*override*/
{
	// player
	if (pOther->GetType() == ObjectType::OBJ_PLAYER)
	{
		if (GetOwner() != pOther)
		{
			SGD::Event* msg = new SGD::Event{"HIT"};
			msg->QueueEvent((Player*)pOther);
			msg = nullptr;
			
			DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
			dMsg->QueueMessage();
			dMsg = nullptr;
		}
	}
	

	// other stuff
	else if (pOther->GetType() == ObjectType::OBJ_BASE || pOther->GetType() == ObjectType::OBJ_WALL)
	{
		DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
		dMsg->QueueMessage();
		dMsg = nullptr;
	}
}

void Bullet::SetOwner(MovingObject* _owner)
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}

	m_pOwner = _owner;
	m_pOwner->AddRef();
}

bool Bullet::IsDead()
{
	SGD::Rectangle world = { 0, 0,
		GameplayState::GetInstance()->GetWorldSize().width,
		GameplayState::GetInstance()->GetWorldSize().height };

	if (GetRect().IsIntersecting(world))
		return false;
	else
		return true;

}