#pragma once
#include "..\SGD Wrappers\SGD_Message.h"

class Player;

class CreateShotgunBullet : public SGD::Message
{
public:
	CreateShotgunBullet(Player*);
	virtual ~CreateShotgunBullet();

	Player* GetOwner(void) const { return m_pOwner; }

private:
	Player* m_pOwner = nullptr;
};

