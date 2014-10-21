#include "CreatePistolBullet.h"
#include "MessageID.h"
#include "MovingObject.h"

CreatePistolBullet::CreatePistolBullet(MovingObject* owner) : Message(MessageID::MSG_CREATE_PSTL_BLT)
{
	m_pOwner = owner;
	m_pOwner->AddRef();

}


CreatePistolBullet::~CreatePistolBullet()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}
