#pragma once
#include "..\SGD Wrappers\SGD_Message.h"

class MovingObject;

class CreateShotgunBullet : public SGD::Message
{
public:
	CreateShotgunBullet(MovingObject*);
	virtual ~CreateShotgunBullet();

	MovingObject* GetOwner(void) const { return m_pOwner; }

private:
	MovingObject* m_pOwner = nullptr;
};

