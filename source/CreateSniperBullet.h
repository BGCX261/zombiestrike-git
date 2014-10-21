#pragma once
#include "..\SGD Wrappers\SGD_Message.h"


class MovingObject;

class CreateSniperBullet : public SGD::Message
{
public:

	CreateSniperBullet(MovingObject*);
	virtual ~CreateSniperBullet();

	MovingObject* GetOwner(void) const { return m_pOwner; }

private:
	MovingObject* m_pOwner = nullptr;

};

