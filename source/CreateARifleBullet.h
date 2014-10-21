#pragma once
#include "..\SGD Wrappers\SGD_Message.h"
class MovingObject;


class CreateARifleBullet : public SGD::Message
{
public:
	CreateARifleBullet(MovingObject*);
	virtual ~CreateARifleBullet();

	MovingObject* GetOwner(void) const { return m_pOwner; }

private:
	MovingObject* m_pOwner = nullptr;
};

