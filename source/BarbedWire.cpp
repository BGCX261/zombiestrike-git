#include "BarbedWire.h"


BarbedWire::BarbedWire()
{
	this->SetType(ObjectType::OBJ_BARBEDWIRE);
	this->SetAnimation("barbedwire");

	m_fCurrHP = m_fMaxHP = 100.0f;

	m_fDamage = 10.0f;
}

BarbedWire::~BarbedWire()
{
}

void BarbedWire::Update( float dt )
{
	if (this->isActive == false)
		return;


	SandBag::Update(dt);
}

void BarbedWire::Render( void )
{
	if (this->isActive == false)
		return;


	SandBag::Render();
}

void BarbedWire::HandleCollision( const IBase* pOther )
{
	if (this->isActive == false)
		return;


	// zombie collision
	if (pOther->GetType() == ObjectType::OBJ_ZOMBIE)
	{
		// lower zombie HP by damage

	}


	// Let the SandBag handle damage to "this"
	SandBag::HandleCollision(pOther);
}