#pragma once

#include "MovingObject.h"
#include "../SGD Wrappers/SGD_Handle.h"	
#include <string>

class MovingObject;

class Bullet : public MovingObject
{
public:
	Bullet() = default;
	virtual ~Bullet();

	virtual void		Update(float elapsedTime) override;
	virtual void		HandleCollision(const IBase* pOther)	override;

	virtual int			GetType(void) const override { return OBJ_BULLET; }

	MovingObject*			GetOwner(void) const { return m_pOwner; }

	//Mutators
	void				SetOwner(MovingObject* _owner);
	bool				IsDead();

private:

	MovingObject* m_pOwner = nullptr;

	friend class BaseBehavior;


};

