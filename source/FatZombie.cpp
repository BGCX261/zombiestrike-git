#include "FatZombie.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"
#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "AnimationManager.h"
#include "Bullet.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "DestroyObjectMessage.h"
#include "Weapon.h"
#include "PukerBlaster.h"
#include "SpawnManager.h"


FatZombie::FatZombie()
{
	health = 200.0f;
	damage = 20.0f;
	//damage = 3.0f;
	//pukeBlaster = new PukerBlaster(this);
}


FatZombie::~FatZombie()
{
	//pukeBlaster->SetOwner(nullptr);
	//delete pukeBlaster;
	//pukeBlaster = nullptr;
}
void FatZombie::Update(float dt)
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	pukeBlaster.Update(dt);
	if (isAlive)
	{
		if (currBehavior != nullptr)
			currBehavior->Update(dt, this, m_pTarget->GetPosition());

		// possible turret target
		SGD::Event event = { "ASSESS_THREAT", nullptr, this };
		event.SendEventNow(nullptr);

		if ((m_pTarget->GetPosition() - m_ptPosition).ComputeLength() <= 200.0f)
		{

			pukeBlaster.Fire(dt);
		}
	}
	else
	{
		DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
		dMsg->QueueMessage();
		dMsg = nullptr;

		pukeBlaster.SetOwner(nullptr);

		SpawnManager::GetInstance()->SetEnemiesKilled(SpawnManager::GetInstance()->GetEnemiesKilled() + 1);

	}
	MovingObject::Update(dt);
}


/*virtual*/ void FatZombie::HandleEvent(const SGD::Event* pEvent)
{

}

