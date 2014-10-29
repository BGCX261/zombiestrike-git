#pragma once

#include "Map.h"

#include "IBase.h"
#include "Graph.h"
#include "GamerProfile.h"
#include <string>
#include <map>

#define NUM_LEVELS 6
using namespace std;
class BaseObject;
class EntityManager;
class Player;
class EnemyUnit;


struct TileSets
{
	Tile pTiles;
	Tile edges;
	Layer layers;
	SGD::Size tileSize;
	Map map;
};


class MapManager : public IBase
{
	TileSets tStruct;
	SGD::HTexture tileTexture = SGD::INVALID_HANDLE;
	std::string levels[NUM_LEVELS];
	int startRow, startCol, endRow, endCol;
	
public:

	static MapManager * GetInstance(void);

	BaseObject*				LoadLevel		( GamerProfile& currProfile, EntityManager* );
	void					UnloadLevel		(void);

	virtual void			Render			(void)							override;
	virtual void			Update			(float elapsedTime)				override;
	virtual void			HandleCollision	(const IBase* pOther)			override;
	virtual int				GetType			(void)					const	override	{ return 10; }
	virtual SGD::Rectangle	GetRect			(void)					const	override	{ return SGD::Rectangle(0.0f, 0.0f, 0.0f, 0.0f); }
	virtual void			AddRef			(void)							override	{};
	virtual void			Release			(void)							override	{};


	SGD::HTexture			GetMapTexture	(void)					{ return tileTexture; }
	void					SetMapTexture	(SGD::HTexture hText)	{ tileTexture = hText; }

	void					UnloadTexture	(SGD::HTexture);
	void					LoadLevelPaths	(void);


	Player* CreatePlayer(int type, SGD::Point pos, EntityManager*);
	void CreateLandMine(SGD::Point pos,  EntityManager* entities);
	void CreateSandBags(SGD::Point pos, EntityManager* entities);
	void CreateBarbedWire(SGD::Point pos, EntityManager* entities);
	void CreateEnvironment(SGD::Point pos, EntityManager* entities);
	void CreateSpawner(SGD::Point pos, EntityManager* entities);




};



