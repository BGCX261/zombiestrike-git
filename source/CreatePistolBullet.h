#pragma once
#include"../SGD Wrappers\SGD_Message.h"

class Weapon;
class CreatePistolBullet : public SGD::Message
{
public:
	CreatePistolBullet(Weapon*);
	virtual ~CreatePistolBullet();

	Weapon* GetOwner(void) const { return m_pOwner; }

private:
	Weapon* m_pOwner = nullptr;

};

