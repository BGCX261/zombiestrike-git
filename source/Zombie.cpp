#include "Zombie.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameplayState.h"
#include "BehaviorManager.h"
#include "BaseBehavior.h"
#include "AnimationManager.h"
#include "Bullet.h"



Zombie::Zombie() : Listener(this)
{
	RegisterForEvent("BARBWIRE");
	RegisterForEvent("HIT");

}


Zombie::~Zombie() 
{
	UnregisterFromEvent("BARBWIRE");
	UnregisterFromEvent("HIT");
}
void Zombie::Update(float dt)
{
	if (isAlive)
	{
		if (currBehavior != nullptr)
			currBehavior->Update(dt, this, { 0, 0 });
	}
	else
	{

	}
	
}
void Zombie::RetrieveBehavior(std::string name)
{

	if (currBehavior != nullptr)
		currBehavior = nullptr;

	currBehavior = BehaviorManager::GetInstance()->GetBehaviors()[name];
}

/*virtual*/ void Zombie::HandleEvent(const SGD::Event* pEvent)
{

}

/*virtual*/ void Zombie::HandleCollision(const IBase* pOther)
{
	if (pOther->GetType())
	{
		const Bullet* bullet = dynamic_cast<const Bullet*>(pOther);

		health -= bullet->GetDamage();
		if (health <= 0.0f)
		{
			isAlive = false;
		}
	}
}