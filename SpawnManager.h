#pragma once
#include <vector>
#include "Timer.h"

class Spawner;

class SpawnManager
{
public:
	static SpawnManager* GetInstance(void);

	bool		Initialize(void);
	bool		Terminate(void);

	std::vector<Spawner*>& GetSpawnVector() { return spawners; }

	void Update(float dt);

	int GetEnemyType(void) const { return m_nEnemyType; }
	int GetNumEnemies(void) const { return m_nNumEnemies; }
	int GetNumWaveEnemies(void) const { return m_nWaveEnemies; }

	void SetEnemyType(int enemyType){ m_nEnemyType = enemyType; }
	void SetNumEnemies(int enemyNum) { m_nNumEnemies = enemyNum; }
	void SetWaveEnemies(int waveEnemies) { m_nWaveEnemies = waveEnemies; }
	void SetMaxSpawnedEnemies(int maxSpawned) { m_nMaxEnemies = maxSpawned; }



private:


	SpawnManager() = default;
	~SpawnManager() = default;

	SpawnManager(const SpawnManager&) = delete;
	SpawnManager&	operator=	(const SpawnManager&) = delete;
	

	int m_nWaveEnemies = 0;
	int m_nMaxEnemies = 0;
	int m_nNumEnemies = 0;
	int m_nEnemyType = 0;
	Timer m_fSpawnTimer;
	bool isActive = false;


	std::vector<Spawner*> spawners;

};

