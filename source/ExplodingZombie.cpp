#include "ExplodingZombie.h"
#include "BaseBehavior.h"
#include "DestroyObjectMessage.h"
#include "SpawnManager.h"


ExplodingZombie::ExplodingZombie()
{
	health = 60.0f;
	damage = 200.0f;
}


ExplodingZombie::~ExplodingZombie()
{

}

void ExplodingZombie::Update(float dt)
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
}


void ExplodingZombie::HandleEvent(const SGD::Event* pEvent)
{

}