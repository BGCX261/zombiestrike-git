#include "Turret.h"
#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "GameplayState.h"

#include "CreateARifleBullet.h"
#include "DestroyObjectMessage.h"


#include "BaseObject.h"
#include "Player.h"
#include "Zombie.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "WeaponManager.h"
#include "HTPGameState.h"


Turret::Turret() : Listener(this)
{
	RegisterForEvent("TARGET_FOUND");
	RegisterForEvent("LOST_TARGET");
	RegisterForEvent("UNPAUSE");
	RegisterForEvent("PAUSE");
	RegisterForEvent("ASSESS_THREAT");

	fireSound = &GameplayState::GetInstance()->rifle_fire;

	this->SetAnimation("turret");

	this->bulletmaker = WeaponManager::GetInstance()->CreateAssaultRifle();
	this->bulletmaker->SetOwner(this);
}

Turret::~Turret()
{
	UnregisterFromEvent("TARGET_FOUND");
	UnregisterFromEvent("LOST_TARGET");
	UnregisterFromEvent("UNPAUSE");
	UnregisterFromEvent("PAUSE");

	fireSound = nullptr;

	SetTarget(nullptr);

	this->bulletmaker->SetOwner(nullptr);
	delete bulletmaker;

	//delete bulletmaker;
	SetWeapon(nullptr);

	SetOwner(nullptr);

}

/*virtual*/ void Turret::Update(float dt)	 /*override*/
{
	if (isActive == false)
		return;

	// update weapon
	if (bulletmaker != nullptr)
	{
		bulletmaker->Update(dt);


		// if out of ammo
		if (bulletmaker->GetTotalAmmo() == 0)
		{
			// turn off turet
			isActive = false;
			DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
			dMsg->QueueMessage();
			dMsg = nullptr;


			// tell owner
			//m_pOwner->SetNumTurrets(m_pOwner->GetNumTurrets() - 1);
		}
	}


	// check if target is still alive
	if (m_pTarget != nullptr && m_pTarget->IsAlive() == false)
	{
		SetTarget(nullptr);
	}


	// update turret
	if (currBehavior != nullptr)
	{
		if (m_pTarget != nullptr)
		{
			if (currBehavior->Update(dt, this, m_pTarget->GetPosition()) == false)
			{
				if (bulletmaker != nullptr && bulletmaker->GetCurrAmmo() > 0)
					bulletmaker->Fire(dt);
			}
		}
	}
	





	MovingObject::Update(dt);
}

/*virtual*/ void Turret::Render()
{
	if (isActive == false)
		return;

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	if (m_pTarget != nullptr)
	{
		GameCamera* camera;
		if (Game::GetInstance()->GetCurrState() == GameplayState::GetInstance())
		{
			camera = GameplayState::GetInstance()->GetCamera();
		}

		else
			camera = HTPGameState::GetInstance()->GetCamera();


		// Draw the green line
		SGD::Point drawpos;
		drawpos.x = (m_ptPosition.x) - camera->GetPosition().x;
		drawpos.y = (m_ptPosition.y) - camera->GetPosition().y;

		SGD::Point targetpos;
		targetpos.x = (m_pTarget->GetPosition().x) - camera->GetPosition().x;
		targetpos.y = (m_pTarget->GetPosition().y) - camera->GetPosition().y;

		pGraphics->DrawLine(drawpos, targetpos, { 255, 0, 255, 0 }, 2u);
	}


	MovingObject::Render();
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
		//if (isActive == true && m_pTarget != nullptr)
		//	pAudio->PlayAudio(*fireSound, false);
	}
	else if (pEvent->GetEventID() == "ASSESS_THREAT" && m_pTarget == nullptr)
	{
		Zombie * sender = reinterpret_cast<Zombie*>(pEvent->GetSender());
		SGD::Vector vToSender = sender->GetPosition() - this->m_ptPosition;

		float range = vToSender.ComputeLength();
		if (range < 600.0F)
		{
			vToSender.Normalize();

			SGD::Vector vAIOrientation = { 0, -1 };		// default = up
			vAIOrientation.Rotate(m_fRotation);			// rotate

			if (vAIOrientation.ComputeDotProduct(vToSender) < 0.86F)
				SetTarget(reinterpret_cast<Zombie*>(pEvent->GetSender()));
		}
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
}

void Turret::SetTarget(Zombie * ptr)
{
	if( m_pTarget != nullptr )
		m_pTarget->Release();

	m_pTarget = ptr;

	if( m_pTarget != nullptr )
		m_pTarget->AddRef();
}

void Turret::SetOwner(MovingObject* owner)
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}

	m_pOwner = (Player*)owner;

	if (m_pOwner != nullptr)
		m_pOwner->AddRef();

}
