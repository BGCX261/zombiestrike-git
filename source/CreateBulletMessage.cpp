#include "CreateBulletMessage.h"
#include "MessageID.h"
#include "MovingObject.h"

CreateBulletMessage::CreateBulletMessage(MovingObject* owner) : Message(MessageID::MSG_CREATE_BULLET)
{
	m_pOwner = owner;
	m_pOwner->AddRef();

}


CreateBulletMessage::~CreateBulletMessage()
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}
