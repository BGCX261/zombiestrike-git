#pragma once
#include "Zombie.h"
class FatZombie :
	public Zombie
{
public:
	FatZombie();
	virtual ~FatZombie();

	virtual void Update(float);

	virtual int GetType(void) const override { return OBJ_FAT_ZOMBIE; }
	virtual void HandleEvent(const SGD::Event* pEvent) override;
	virtual void HandleCollision(const IBase* pOther) override;

};

