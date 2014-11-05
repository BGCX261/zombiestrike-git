#include "Bullet.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "BaseObject.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "AnimationManager.h"
#include "DestroyObjectMessage.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


Bullet::Bullet() : Listener(this)
{
	RegisterForEvent("KILL_ME");
}

Bullet::~Bullet()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
	
	UnregisterFromEvent("KILL_ME");
}

/*virtual*/ void Bullet::Update(float dt) /*override*/
{
	//MovingObject::Update(dt);
	//lifeTime -= m_vtVelocity.ComputeLength() * dt;
	m_ptPosition += m_vtVelocity * dt;
	AnimationManager::GetInstance()->Update(animation, dt, this);

	if (IsDead())
	{
	

		DestroyObjectMessage pMsg(this);
		pMsg.SendMessageNow();
		
		
	
	}
}

/*virtual*/ void Bullet::HandleCollision(const IBase* pOther)	/*override*/
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	if (this->type != ObjectType::OBJ_VOMIT)
	{
		// player
		if (pOther->GetType() == OBJ_SLOW_ZOMBIE ||
			pOther->GetType() == OBJ_FAST_ZOMBIE ||
			pOther->GetType() == OBJ_FAT_ZOMBIE ||
			pOther->GetType() == OBJ_TANK_ZOMBIE ||
			pOther->GetType() == OBJ_EXPLODING_ZOMBIE)
		{
			if (GetOwner() != pOther)
			{
				if (pAudio->IsAudioPlaying(GameplayState::GetInstance()->bullet_hit_zombie) == false)
					pAudio->PlayAudio(GameplayState::GetInstance()->bullet_hit_zombie, false);

				DestroyObjectMessage pMsg(this);
				pMsg.SendMessageNow();
			}
		}
	}
	else if (this->type == ObjectType::OBJ_VOMIT)
	{
		// zombie
		if (pOther->GetType() == OBJ_PLAYER)
		{
			if (GetOwner() != pOther)
			{
				if (pAudio->IsAudioPlaying(GameplayState::GetInstance()->vomit_hit_player) == false)
					pAudio->PlayAudio(GameplayState::GetInstance()->vomit_hit_player, false);

	
				DestroyObjectMessage pMsg(this);
				pMsg.SendMessageNow();
			}
		}
	}

	// other stuff
	else if (pOther->GetType() == ObjectType::OBJ_BASE || pOther->GetType() == ObjectType::OBJ_WALL)
	{
		DestroyObjectMessage pMsg(this);
		pMsg.SendMessageNow();
	}
}

/*virtual*/ void Bullet::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "KILL_ME")
	{
		
		DestroyObjectMessage pMsg(this);
		pMsg.SendMessageNow();
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
	SGD::Rectangle rect = GetRect();
	rect.Offset(GameplayState::GetInstance()->GetCamera()->GetPosition().x, GameplayState::GetInstance()->GetCamera()->GetPosition().y);
	if (rect.IsIntersecting(world))
		return false;
	else
		return true;

}