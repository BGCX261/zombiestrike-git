#pragma once
#include"../SGD Wrappers\SGD_Message.h"

class MovingObject;
class CreatePistolBullet : public SGD::Message
{
public:
	CreatePistolBullet(MovingObject*);
	virtual ~CreatePistolBullet();

	MovingObject* GetOwner(void) const { return m_pOwner; }

private:
	MovingObject* m_pOwner = nullptr;

};

