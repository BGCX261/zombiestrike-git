#include "P90.h"
#include "MovingObject.h"

P90::P90(MovingObject* owner)
{
	m_pOwner = owner;
	m_pOwner->AddRef();
}


P90::~P90()
{
	/*m_pOwner->Release();
	m_pOwner = nullptr;*/
}

void P90::Fire(float dt)
{


}
