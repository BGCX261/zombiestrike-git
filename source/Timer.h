#pragma once


class Timer
{
	float m_fCurrTime = 0;
	float maxTime = -1.0f;
public:
	void AddTime(float time) { m_fCurrTime += time; }
	void SetMaxTime(float _maxTime) { maxTime = _maxTime; }
	bool Update(float dt);

	

};

