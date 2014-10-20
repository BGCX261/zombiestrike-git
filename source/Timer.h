#pragma once


class Timer
{
	float m_fCurrTime = 0;
public:
	void AddTime(float time) { m_fCurrTime += time; }
	bool Update(float dt);
	

};

