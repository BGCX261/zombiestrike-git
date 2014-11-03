#pragma once
#include "Weapon.h"
class PukerBlaster : public Weapon
{
public:
	PukerBlaster(MovingObject* owner = nullptr);
	virtual ~PukerBlaster();
	virtual void Fire(float dt) override;
};

