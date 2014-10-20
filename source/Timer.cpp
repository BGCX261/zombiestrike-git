#include "Timer.h"


bool Timer::Update(float dt)
{
	if (m_fCurrTime > 0.0f)
	{
		m_fCurrTime -= dt;
		return false;

	}
	else 
	{
		if (m_fCurrTime < 0.0f)
			m_fCurrTime = 0.0f;
		return true;
	}

}