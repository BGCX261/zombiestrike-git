#include "SandBag.h"


SandBag::SandBag()
{
	this->SetType(ObjectType::OBJ_SANDBAG);
	this->SetAnimation("sandbag");

	m_fCurrHP = m_fMaxHP = 100.0f;
}

SandBag::~SandBag()
{
}

void SandBag::Update( float dt )
{
	if (this->isActive == false)
		return;


	BaseObject::Update(dt);
}

void SandBag::Render( void )
{
	if (this->isActive == false)
		return;


	BaseObject::Render();
}

void SandBag::HandleCollision( const IBase* pOther )
{
	if (this->isActive == false)
		return;


	// zombie collision
	if (pOther->GetType() == ObjectType::OBJ_ZOMBIE)
	{
		// lower HP by zombie damage
		m_fCurrHP -= 1.0f;


		// dead
		if (m_fCurrHP <= 0.0f)
		{
			// reset HP
			m_fCurrHP = m_fMaxHP;

			// deactivate
			this->isActive = false;
		}
	}

}