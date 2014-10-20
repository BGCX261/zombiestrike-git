#pragma once
#include "MovingObject.h"
#include "../SGD Wrappers\SGD_Listener.h"
#include "Timer.h"

class Player;

class Zombie : public MovingObject, public SGD::Listener
{
public:
	Zombie();
	~Zombie();

	virtual void Update(float dt)	 override;
	
	virtual void HandleEvent(const SGD::Event* pEvent);
	virtual void HandleCollision(const IBase* pOther) override;

	virtual int GetType(void) const override { return ObjectType::OBJ_ZOMBIE; }

	Player*		GetTarget(void) { return m_pTarget; }
	void		SetTarget(Player* _target);
	void		RetrieveBehavior(std::string name);



	friend class BaseBehavior;


private:


	BaseBehavior* currBehavior = nullptr;
	bool isAlive = true;
	Timer m_tStunTimer;
	Player* m_pTarget = nullptr;

	
};

