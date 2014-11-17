#include "Grenade.h"
#include "DestroyObjectMessage.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "House.h"
#include "Game.h"
#include "AnimationManager.h"
#include "Animation.h"


Grenade::Grenade()
{
}

Grenade::~Grenade()
{
}

void Grenade::Update(float dt)
{
	/*
	speed -= 600.0f * dt;
	m_vtVelocity = direction * speed;
	MovingObject::Update(dt);
	if (speed > 375.0f)
		m_szScale += SGD::Size(1.0f, 1.0f) * dt;
	else
		m_szScale -= SGD::Size(1.0f, 1.0f) * dt;

	
	if (speed < 0)
	{
		speed = 0;
		SetAnimation("explosion");
	}
	*/

	MovingObject::Update(dt);



	// on last frame of expolsion animation
	int numframes = AnimationManager::GetInstance()->GetAnimation("explosion")->GetFrames().size();
	numframes--;

	if ((this->GetAnimation() == "explosion" && this->GetAnimationStamp().m_nCurrFrame == numframes) || IsDead())
	{
		DestroyObjectMessage* pMsg = new DestroyObjectMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}	

}

/*virtual*/ void Grenade::HandleCollision(const IBase* pOther)	/*override*/
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();



	// zombies
	if (pOther->GetType() >= OBJ_SLOW_ZOMBIE && pOther->GetType() <= OBJ_TANK_ZOMBIE)
	{
		if (GetOwner() != pOther)
		{
			pAudio->PlayAudio(Game::GetInstance()->explosion, false);

			this->SetVelocity({ 0, 0 });

			if (this->GetAnimation() != "explosion")
				this->SetAnimation("explosion");
		}
	}


	// other stuff
	else if (pOther->GetType() == ObjectType::OBJ_BASE || pOther->GetType() == ObjectType::OBJ_WALL)
	{
		const House* house = dynamic_cast<const House*>(pOther);

		if (house->IsActive() == true)
		{
			pAudio->PlayAudio(Game::GetInstance()->explosion, false);

			this->SetVelocity({ 0, 0 });

			if (this->GetAnimation() != "explosion")
				this->SetAnimation("explosion");
		}
	}


}

