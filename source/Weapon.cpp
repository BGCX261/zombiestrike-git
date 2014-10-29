#include "Weapon.h"
#include "MovingObject.h"

Weapon::Weapon()
{

}


Weapon::~Weapon()
{
	SetOwner(nullptr);
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
	//if (currAmmo > 0)
	//{
	//	//create bullet message
	//	CreateBulletMessage* pMsg = new CreateBulletMessage(m_pOwner);
	//	pMsg->QueueMessage();
	//	pMsg = nullptr;

	//	recoilTimer.AddTime(recoilTime);
	//	currAmmo--; 
	//	if (currAmmo == 0)
	//		reloadTimer.AddTime(reloadTime);
	//}

}
void Weapon::SetOwner(MovingObject* owner)
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr; 
	}
	m_pOwner = owner;

}
