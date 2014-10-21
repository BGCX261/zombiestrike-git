#include "Weapon.h"
#include "Player.h"
#include "CreateBulletMessage.h"

Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}
 void Weapon::Update(float dt)
{
	 recoilTimer.Update(dt);
	 if (currAmmo == 0)
	 {
		 if (reloadTimer.Update(dt))
			 currAmmo = magSize;
	 }


}

void Weapon::Fire(float dt)
{
	if (currAmmo > 0)
	{
		//create bullet message
		CreateBulletMessage* pMsg = new CreateBulletMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;

		recoilTimer.AddTime(recoilTime);
		currAmmo--; 
		if (currAmmo == 0)
			reloadTimer.AddTime(reloadTime);
	}

}
void Weapon::SetOwner(Player* owner)
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr; 
	}
	m_pOwner = owner;

}
