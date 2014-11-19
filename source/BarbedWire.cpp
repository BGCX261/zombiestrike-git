#include "BarbedWire.h"
#include "../SGD Wrappers/SGD_Event.h"

BarbedWire::BarbedWire()
{
	
	

	m_fCurrHP = m_fMaxHP = 50.0f;

	m_fDamage = 2.0f;

	RegisterForEvent("REPAIR_BARBEDWIRE");
	RegisterForEvent("UPGRADE_BARBEDWIRE_HEALTH");
	RegisterForEvent("UPGRADE_BARBEDWIRE_DAMAGE");

}

BarbedWire::~BarbedWire()
{
}

void BarbedWire::Update( float dt )
{

	SandBag::Update(dt);
}

void BarbedWire::Render( void )
{

	SandBag::Render();
}

void BarbedWire::HandleCollision( const IBase* pOther )
{


	SandBag::HandleCollision(pOther);





	// Let the SandBag handle damage to "this"
}

void BarbedWire::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "REPAIR_BARBEDWIRE")
	{
		isActive = true;
		m_fCurrHP = m_fMaxHP;

	}
	if (pEvent->GetEventID() == "UPGRADE_BARBEDWIRE_HEALTH")
	{
		m_fMaxHP += 50;
		m_fCurrHP = m_fMaxHP;
	}
	if (pEvent->GetEventID() == "UPGRADE_BARBEDWIRE_DAMAGE")
	{
		m_fDamage += 5.0f;
	}
}
