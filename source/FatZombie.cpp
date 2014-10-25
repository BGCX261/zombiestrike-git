#include "FatZombie.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"
#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "AnimationManager.h"
#include "Bullet.h"
#include "DestroyObjectMessage.h"


FatZombie::FatZombie()
{
	health = 200.0f;
	damage = 20.0f;


}


FatZombie::~FatZombie()
{
	
}
void FatZombie::Update(float dt)
{
	if (isAlive)
	{
		if (currBehavior != nullptr)
			currBehavior->Update(dt, this, { 0, 0 });
	}
	else
	{
		DestroyObjectMessage* dMsg = new DestroyObjectMessage{ this };
		dMsg->QueueMessage();
		dMsg = nullptr;
	}

}


/*virtual*/ void FatZombie::HandleEvent(const SGD::Event* pEvent)
{

}

/*virtual*/ void FatZombie::HandleCollision(const IBase* pOther)
{
	if (pOther->GetType() == OBJ_BULLET)
	{
		const Bullet* bullet = dynamic_cast<const Bullet*>(pOther);

		health -= bullet->GetDamage();
		if (health <= 0.0f)
		{
			isAlive = false;
		}
	}

}