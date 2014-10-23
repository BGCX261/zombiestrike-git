#pragma once
#include <string>


struct GamerProfile
{
	float MaxEnergy = 100.0f;
	float MaxStamina = 100.0f;
	float CamoMultiplier = 1.0f;
	float SpeedMultiplier = 1.0f;
	float RadarMultiplier = 1.0f;
	int LevelsComplete = 0;
	std::string path;
	bool m_bHasKey = false;
	bool CheckPointReached = false;


};