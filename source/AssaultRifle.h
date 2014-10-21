#pragma once
#include "Weapon.h"
class AssaultRifle : public Weapon
{
public:
	AssaultRifle();
	virtual ~AssaultRifle();
	virtual void Fire(float dt) override;
};

