#include "CreateARifleBullet.h"
#include "MessageID.h"
#include "MovingObject.h"

CreateARifleBullet::CreateARifleBullet(MovingObject* owner) : Message(MessageID::MSG_CREATE_ASSRFLE_BLT)
{
	m_pOwner = owner;
	m_pOwner->AddRef();
}


CreateARifleBullet::~CreateARifleBullet()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}
