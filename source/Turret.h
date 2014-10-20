#pragma once
#include "MovingObject.h"
#include "../SGD Wrappers/SGD_Listener.h"

class BaseBehavior;

class Turret : public MovingObject, public SGD::Listener
{
public:
	Turret();
	~Turret();

	virtual void	Update				(float dt)							override;
//	virtual void	Render				(void)								override;
	virtual void	HandleEvent			(const SGD::Event* pEvent);
	virtual void	HandleCollision		(const IBase* pOther)				override;

	virtual void	Attack				(void)								override;
	virtual int		GetType				(void)						const	override	{ return ObjectType::OBJ_TURRET; }
	void			SetShutDownEvent	(const char* pEvent)							{ ShutDownEvent = pEvent; }
	void			RetrieveBehavior	(std::string name);


private:

	BaseBehavior*	currBehavior	= nullptr;
	SGD::HAudio*	fireSound		= nullptr;
	bool			isActive		= true;
	std::string		ShutDownEvent;


	friend class BaseBehavior;
};

