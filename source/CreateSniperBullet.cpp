#include "CreateSniperBullet.h"
#include "MessageID.h"
#include "MovingObject.h"

CreateSniperBullet::CreateSniperBullet(MovingObject* owner) : Message(MessageID::MSG_CREATE_SNPR_BLT)
{
	m_pOwner = owner;
	m_pOwner->AddRef();
}


CreateSniperBullet::~CreateSniperBullet()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}
