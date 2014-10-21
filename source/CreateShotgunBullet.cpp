#include "CreateShotgunBullet.h"
#include "MessageID.h"

#include "Player.h"

CreateShotgunBullet::CreateShotgunBullet(Player* owner) : Message(MessageID::MSG_CREATE_BULLET)
{
	m_pOwner = owner;
	m_pOwner->AddRef();
}


CreateShotgunBullet::~CreateShotgunBullet()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
	
}
