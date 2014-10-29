#include "MapManager.h"
#include "Map.h"
#include "SpawnManager.h"
#include <fstream>
#include "BaseObject.h"
#include "../TinyXML/tinyxml.h"
#include "GameplayState.h"
#include "Game.h"
#include "Player.h"
#include "BarbedWire.h"
#include "SandBag.h"
#include "LandMine.h"
#include "Spawner.h"
#include "EntityManager.h"
//#include "../resource/config/"


enum EntityBucket {  BUCKET_ENEMIES, BUCKET_PLAYER, BUCKET_ENVIRO, BUCKET_BULLETS, BUCKET_SHADOWS, BUCKET_NONCOLLIDABLE };



/*static*/ MapManager * MapManager::GetInstance()
{
	static MapManager sInstance;

	return &sInstance;
}

BaseObject* MapManager::LoadLevel(GamerProfile& currProfile, EntityManager* m_pEntities)
{
	//EntityManager* m_pEntities = new EntityManager;
	Player* player;

	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	TiXmlDocument doc;

	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	int posX;
	int posY;
	int rectPosX;
	int rectPosY;
	SGD::Point rPos;
	SGD::Point tPos;
	int tileid = -1;
	SGD::HTexture tSetBG;
	std::string filePath;

	LoadLevelPaths();
	

	/*if (doc.LoadFile(levels[currProfile.LevelsComplete].c_str()) == false)
	{
		return nullptr;
	}*/
	if (doc.LoadFile(levels[0].c_str()) == false)
	{
		return nullptr;
	}
	TiXmlElement * root = doc.RootElement();
	if (root == nullptr)
	{
		return nullptr;
	}

	TiXmlElement * level = root->FirstChildElement("world_info");

	level->Attribute("width", &tileWidth);
	level->Attribute("height", &tileHeight);
	level->Attribute("worldWidth", &mapWidth);
	level->Attribute("worldHeight", &mapHeight);

	tStruct.tileSize.width = (float)tileWidth;
	tStruct.tileSize.height = (float)tileHeight;
	tStruct.map.SetMapWidth(mapWidth);
	tStruct.map.SetMapHeight(mapHeight);
	const int cMapWidth = tStruct.map.GetMapWidth();
	const int cMaHeight = tStruct.map.GetMapHeight();


	tStruct.layers.m_vTiles = new Tile*[cMaHeight];

	for (int i = 0; i < cMaHeight; i++)
		tStruct.layers.m_vTiles[i] = new Tile[cMapWidth];





	const char * fpText = level->GetText();
	if (fpText != nullptr)
	{
		string fullPath = fpText;
		int startIndex = fullPath.find_last_of('\\');

		filePath += fullPath.substr(startIndex + 1, fullPath.length());
		string path = "resource/graphics/";
		path += filePath;
		tStruct.map.SetTilePath(path);
	}

	TiXmlElement * playerInfo = root->FirstChildElement("player_info");



	playerInfo->Attribute("id", &tileid);
	playerInfo->Attribute("posX", &posX);
	playerInfo->Attribute("posY", &posY);

	/*player = CreatePlayer(tileid, { (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
	player->SetGamerProfile(currProfile);*/
	//level = level->NextSiblingElement("tile_info");
	TiXmlElement * tileList = root->FirstChildElement("tile_list");
	TiXmlElement * tileinfo = tileList->FirstChildElement("tile_info");

	while (tileinfo != nullptr)
	{
		tileinfo->Attribute("rposx", &rectPosX);
		tileinfo->Attribute("rposy", &rectPosY);
		tileinfo->Attribute("posX", &posX);
		tileinfo->Attribute("posY", &posY);


		Tile temp;

		temp.worldPos.x = (float)posX;
		temp.worldPos.y = (float)posY;

		temp.renderPos.x = (float)rectPosX;
		temp.renderPos.y = (float)rectPosY;
		


		tStruct.layers.m_vTiles[posY / tileHeight][posX / tileWidth] = temp;
		tileinfo = tileinfo->NextSiblingElement("tile_info");
	}



	TiXmlElement * collisionList = root->FirstChildElement("collsion_list");
	TiXmlElement * collisionInfo = collisionList->FirstChildElement("collision_tile");

	while (collisionInfo != nullptr)
	{
		collisionInfo->Attribute("posX", &posX);
		collisionInfo->Attribute("posY", &posY);
		collisionInfo->Attribute("type", &tileid);

		CreateEnvironment({ (float)posX * tileWidth, (float)posY * tileHeight } ,m_pEntities);


		collisionInfo = collisionInfo->NextSiblingElement("collision_tile");
	}

	//Currently unneeded pathfinding stuff
#if 0
	TiXmlElement * graphList = root->FirstChildElement("graph_list");
	TiXmlElement * nodeInfo = graphList->FirstChildElement("node_info");

	while (nodeInfo != nullptr)
	{
	int numEdges = 0;
	int edge;

	std::string edgeNode;

	nodeInfo->Attribute("posX", &posX);
	nodeInfo->Attribute("posY", &posY);
	nodeInfo->Attribute("index", &tileid);
	nodeInfo->Attribute("numEdges", &numEdges);
	const char* tagCheck = nodeInfo->GetText();
	if (tagCheck != nullptr)
	{
	string tag = tagCheck;
	if (tag == "alarm")
	alarmNode = tileid;
	if (tag == "home")
	homeNode = tileid;
	}


	SGD::Point* temp = new SGD::Point((float)posX * tileWidth + (tileWidth *0.5f), (float)posY * tileHeight + (tileHeight *0.5f));
	navGraph.addVertex(temp);


	for (int currEdge = 0; currEdge < numEdges; currEdge++)
	{
	edgeNode = "edge";
	edgeNode += std::to_string(currEdge);
	nodeInfo->Attribute(edgeNode.c_str(), &edge);
	if (navGraph.size() == 0)
	break;
	else
	navGraph[navGraph.size() - 1].addEdge(edge);
	}

	nodeInfo = nodeInfo->NextSiblingElement("node_info");
	}
	
 // 
#endif
	

	TiXmlElement * objectList = root->FirstChildElement("objects_list");
	TiXmlElement * objectInfo = objectList->FirstChildElement("object_info");

	while (objectInfo != nullptr)
	{
		std::vector<int> waypoints;
		std::string Event1;
		std::string Event2;
		const char* NameandEvents;
		int numwayPoints = 0;
		int numEnemies = 0;
		int startState = 0;

		objectInfo->Attribute("id", &tileid);
		objectInfo->Attribute("numEnemies", &numEnemies);
		objectInfo->Attribute("startState", &startState);
		objectInfo->Attribute("posX", &posX);
		objectInfo->Attribute("posY", &posY);
		objectInfo->Attribute("numWaypoints", &numwayPoints);

		NameandEvents = objectInfo->GetText();

		for (int i = 0; i < numwayPoints; i++)
		{
			int index = 0;
			std::string waypoint = "waypoint";
			waypoint += std::to_string(i);
			objectInfo->Attribute(waypoint.c_str(), &index);

			waypoints.push_back(index);
		}

		


		switch (tileid)
		{
			case BaseObject::OBJ_SANDBAG:
				CreateSandBags({ (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
				break;
			case BaseObject::OBJ_BARBEDWIRE:
				CreateBarbedWire({ (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
				break;
			case BaseObject::OBJ_LANDMINE:
				CreateLandMine({ (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
				break;
			case BaseObject::OBJ_SPAWNER:
				CreateSpawner({ (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
				break;
		};


		objectInfo = objectInfo->NextSiblingElement("object_info");

	}

	


	//Player* player = dynamic_cast<Player*>(m_pPlayer);

	

	tileTexture = pGraphics->LoadTexture(tStruct.map.GetTilePath().c_str());


	return nullptr;
}
void MapManager::UnloadLevel(void)
{

	for (unsigned int i = 0; i < tStruct.map.GetMapHeight(); i++)
	{
		delete[] tStruct.layers.m_vTiles[i];
	}
	delete [] tStruct.layers.m_vTiles;

}

void MapManager::Render()
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();
	
	for (int currRow = startRow; currRow < endRow; currRow++)
	{
		for (int currCol = startCol; currCol < endCol; currCol++)
		{
			SGD::Point point = tStruct.layers.m_vTiles[currRow][currCol].worldPos;
			point.Offset({ -GameplayState::GetInstance()->GetCamera()->GetPosition().x, -GameplayState::GetInstance()->GetCamera()->GetPosition().y });

			pGraphics->DrawTextureSection(tileTexture,
			 point,
				SGD::Rectangle(tStruct.layers.m_vTiles[currRow][currCol].renderPos, tStruct.tileSize));
		}
	}
}

void MapManager::Update(float elapsedTime)
{
	SGD::Rectangle cameraRect = GameplayState::GetInstance()->GetCamera()->GetRect();

	startCol = (int)cameraRect.left / (int)tStruct.tileSize.width;
	startRow = (int)cameraRect.top / (int)tStruct.tileSize.height;
	endCol = (int)(Game::GetInstance()->GetScreenWidth() + cameraRect.left) / (int)tStruct.tileSize.width;
	endRow = (int)(Game::GetInstance()->GetScreenHeight() + cameraRect.top) / (int)tStruct.tileSize.height;
	endRow++;
	endCol++;
	if (startCol < 0)
	{
		startCol = 0;
	}

	if (startRow < 0)
	{
		startRow = 0;
	}
	if (endCol > (int)tStruct.map.GetMapWidth())
	{
		endCol = (int)tStruct.map.GetMapWidth();
	}

	if (endRow > (int)tStruct.map.GetMapHeight())
	{
		endRow = (int)tStruct.map.GetMapHeight();
	}
}

void MapManager::HandleCollision(const IBase* pOther)
{
	/*for (unsigned int i = 0; i < tStruct.pTiles.GetRectVec().size(); i++)
	{
		if (tStruct.pTiles.GetCollisionID() == 1)
		{

		}
	}*/
}

void MapManager::UnloadTexture(SGD::HTexture)
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	pGraphics->UnloadTexture(GetMapTexture());
}


Player* MapManager::CreatePlayer(int type, SGD::Point pos, EntityManager* entities)
{
	/*Player* player = new Player;
	player->SetPosition(pos);
	player->SetRotation(0.0f);
	player->SetMoveSpeed(180.0f);
	player->RetrieveBehavior("playerController");
	*/


	return nullptr;
}

void MapManager::CreateLandMine(SGD::Point pos, EntityManager* entities)
{
	LandMine* landmine = new LandMine;
	landmine->SetPosition(pos);
	landmine->SetAnimation("testLandmine");
	landmine->SetActive(false);
	entities->AddEntity(landmine, BUCKET_ENVIRO);
	landmine->Release();
	landmine = nullptr;
	
}
void MapManager::CreateSandBags(SGD::Point pos, EntityManager* entities)
{
	SandBag* sandbag = new SandBag;
	sandbag->SetPosition(pos);
	sandbag->SetAnimation("testSandbag");

	entities->AddEntity(sandbag, BUCKET_ENVIRO);
	sandbag->Release();
	sandbag = nullptr;
}
void MapManager::CreateBarbedWire(SGD::Point pos, EntityManager* entities)
{
	BarbedWire* barbedWire = new BarbedWire;
	barbedWire->SetPosition(pos);
	barbedWire->SetAnimation("testBarbwire");

	entities->AddEntity(barbedWire, BUCKET_ENVIRO);
	barbedWire->Release();
	barbedWire = nullptr;
}

void MapManager::CreateSpawner(SGD::Point pos, EntityManager* entities)
{
	Spawner* spawnPoint = new Spawner;
	spawnPoint->SetPosition(pos);
	SpawnManager::GetInstance()->GetSpawnVector().push_back(spawnPoint);
	entities->AddEntity(spawnPoint, BUCKET_NONCOLLIDABLE);

	spawnPoint->Release();
	spawnPoint = nullptr;

}

void MapManager::CreateEnvironment(SGD::Point pos, EntityManager* entities)
{
	
	EnvironmentalObject* object = new EnvironmentalObject;
	object->SetPosition(pos);
	object->SetSize({ 32, 32 });
	
	entities->AddEntity(object, BUCKET_ENVIRO);
	object->Release();
	object = nullptr;
	

}


void  MapManager::LoadLevelPaths(void)
{
	ifstream fin("resource/config/levels/levelPaths.txt", std::ios::in);

	if (fin.is_open())
	{
		unsigned int count = 0;
		for (unsigned int count = 0; !fin.eof() ; count++)
		{
			getline(fin, levels[count]);
		}
	}
}
