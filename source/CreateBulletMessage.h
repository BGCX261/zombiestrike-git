#pragma once
#include"../SGD Wrappers\SGD_Message.h"

class MovingObject;
class CreateBulletMessage : public SGD::Message
{
public:
	CreateBulletMessage(MovingObject*);
	virtual ~CreateBulletMessage();

	MovingObject* GetOwner(void) const { return m_pOwner; }

private:
	MovingObject* m_pOwner = nullptr;

};

