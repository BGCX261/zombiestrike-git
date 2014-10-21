#include "CreateShotgunBullet.h"
#include "MessageID.h"

#include "MovingObject.h"

CreateShotgunBullet::CreateShotgunBullet(MovingObject* owner) : Message(MessageID::MSG_CREATE_SHTGN_BLT)
{
	m_pOwner = owner;
	m_pOwner->AddRef();
}


CreateShotgunBullet::~CreateShotgunBullet()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
	
}
