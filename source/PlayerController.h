#pragma once
#include "BaseBehavior.h"
class PlayerController :
	public BaseBehavior
{
public:
	PlayerController();
	~PlayerController();

	bool Update(float dt, MovingObject* toUpdate, SGD::Point = { 0, 0 });


};

