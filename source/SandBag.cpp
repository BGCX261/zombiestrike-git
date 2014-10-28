#include "SandBag.h"
#include "Game.h"

SandBag::SandBag()
{
	this->SetType(ObjectType::OBJ_SANDBAG);
	this->SetAnimation("sandbag");

	m_fCurrHP = m_fMaxHP = 250.0f;
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
	if (pOther->GetType() == ObjectType::OBJ_SLOW_ZOMBIE ||
		pOther->GetType() == ObjectType::OBJ_FAST_ZOMBIE||
		pOther->GetType() == ObjectType::OBJ_FAT_ZOMBIE)
	{
		// lower HP by zombie damage
		m_fCurrHP -= 10.0f * Game::GetInstance()->DeltaTime();


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