#include "MapManager.h"
#include "Map.h"

#include <fstream>
#include "BaseObject.h"
#include "../TinyXML/tinyxml.h"
#include "GameplayState.h"
#include "Game.h"
#include "Player.h"
//#include "Guard.h"
//#include "Officer.h"
//#include "Worker.h"
//#include "Turret.h"
//#include "SecurityCamera.h"
//#include "SpawnDoor.h"
//#include "PickUp.h"
//#include "EnvironmentalObject.h"
//#include "Computer.h"
//#include "KeyPad.h"
//#include "AlarmButton.h"
//#include "Door.h"
//#include "Shadow.h"

#include "EntityManager.h"
//#include "../resource/config/"


enum EntityBucket {  BUCKET_ENEMIES, BUCKET_PLAYER, BUCKET_ENVIRO, BUCKET_BULLETS, BUCKET_SHADOWS, BUCKET_NONCOLLIDABLE };



/*static*/ MapManager * MapManager::GetInstance()
{
	static MapManager sInstance;

	return &sInstance;
}

BaseObject* MapManager::LoadLevel(GamerProfile& currProfile, EntityManager* m_pEntities, vector<SGD::Rectangle*>& FOVCollidables, Graph<SGD::Point*>& navGraph)
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

	int homeNode, alarmNode;

	SGD::Point m_ptCheckPtStart;

	if (doc.LoadFile(levels[currProfile.LevelsComplete].c_str()) == false)
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

	player = CreatePlayer(tileid, { (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
	player->SetGamerProfile(currProfile);
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
		/*
			tStruct.pTiles.SetTilePos({ tPos.x, tPos.y });
			tStruct.layers.AddRectPos(rPos);
			tStruct.layers.AddTilePos({ tPos.x, tPos.y});
			tStruct.layers.AddTiles(tStruct.pTiles);
			tStruct.pTiles.SetCollisionRect(tPos.x, tPos.y, tPos.x + tileWidth, tPos.y + tileHeight);
			tStruct.pTiles.AddCollisionRect(tStruct.pTiles.GetCollisionRect());
		*/


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

		string Event1;

		const char * NameandEvents = collisionInfo->GetText();
		if (NameandEvents != nullptr)
		{
			std::string events = NameandEvents;
			size_t startIndex = events.find_first_of((char)'//');
			std::string tileEvents = events.substr(0, startIndex);
			Event1 = tileEvents;
			if (Event1 == "CHECKPOINT")
				m_ptCheckPtStart = { (float)posX * tileWidth, (float)posY * tileHeight };
		}


		if (tileid == 0)
			CreateEnvironment(BaseObject::OBJ_WALL, { (float)posX * tileWidth, (float)posY * tileHeight }, FOVCollidables, m_pEntities, Event1.c_str());
		else
			CreateEnvironment(BaseObject::OBJ_TRIGGER, { (float)posX * tileWidth, (float)posY * tileHeight }, FOVCollidables, m_pEntities, Event1.c_str());


		collisionInfo = collisionInfo->NextSiblingElement("collision_tile");
	}



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

		//Depending on the object type, Take in events made
		switch (tileid)
		{
			case BaseObject::OBJ_TURRET:
			{
				std::string events = NameandEvents;
				size_t startIndex = events.find_first_of((char)'//');
				std::string tileEvents = events.substr(startIndex + 1, events.length());
				Event1 = tileEvents;
			}
			break;


			case BaseObject::OBJ_DOOR:
			{
				std::string events = NameandEvents;
				size_t startIndex = events.find_first_of((char)'//');
				std::string tileEvents = events.substr(startIndex + 1, events.length());
				startIndex = tileEvents.find_first_of((char)'//');
				Event1 = tileEvents.substr(0, startIndex);
				Event2 = tileEvents.substr(startIndex + 1, tileEvents.length());
			}
			break;

		};


		switch (tileid)
		{
			case BaseObject::OBJ_TURRET:
				//CreateEnemy(tileid, homeNode, alarmNode, startState, { (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities, player, navGraph, FOVCollidables, waypoints, Event1.c_str());
				break;
			
			case BaseObject::OBJ_POWERCORE:
				//CreatePickUp(tileid, { (float)posX * tileWidth, (float)posY * tileHeight }, m_pEntities);
				break;
		};


		objectInfo = objectInfo->NextSiblingElement("object_info");

	}

	/*for (unsigned int currRect = 0; currRect < tStruct.pTiles.GetRectVec().size(); currRect++)
	{
	if (tStruct.layers.GetTiles()[currRect].GetTileID() == 16)
	FOVCollidables.push_back(new SGD::Rectangle(tStruct.pTiles.GetRectVec()[currRect]));


	}*/


	//Player* player = dynamic_cast<Player*>(m_pPlayer);

	if (currProfile.CheckPointReached == true)
	{
		player->SetPosition(m_ptCheckPtStart);
	}

	tileTexture = pGraphics->LoadTexture(tStruct.map.GetTilePath().c_str());


	return player;
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
	endCol = ((int)Game::GetInstance()->GetScreenWidth() + cameraRect.left) / (int)tStruct.tileSize.width;
	endRow = ((int)Game::GetInstance()->GetScreenHeight() + cameraRect.top) / (int)tStruct.tileSize.height;
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
	if (endCol > tStruct.map.GetMapWidth())
	{
		endCol = tStruct.map.GetMapWidth();
	}

	if (endRow > tStruct.map.GetMapHeight())
	{
		endRow = tStruct.map.GetMapHeight();
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
	Player* player = new Player;
	player->SetPosition(pos);
	player->SetRotation(0.0f);
	player->SetMoveSpeed(180.0f);
	player->RetrieveBehavior("playerController");



	return player;
}
void MapManager::CreateEnvironment(int type, SGD::Point pos, vector<SGD::Rectangle*>& FOVCollidables, EntityManager* entities, const char* event)
{
	/*
	EnvironmentalObject* object = new EnvironmentalObject;
	object->SetPosition(pos);
	if (event != nullptr)
	{
		if (type == BaseObject::OBJ_WALL && event == "PlayerOnly")
		{
			object->SetType(BaseObject::OBJ_PLAYERCOLLISION);
		}
		else
		{
			object->RegisterForEvent(event);
			object->SetEvent(event);
			object->SetType(type);
		}
		
	}
		
	
	object->SetSize({ 64, 64 });
	SGD::Rectangle* collider = new SGD::Rectangle(object->GetRect());

	FOVCollidables.push_back(collider);
	entities->AddEntity(object, BUCKET_ENVIRO);
	object->Release();
	object = nullptr;
	*/

}


void  MapManager::LoadLevelPaths(void)
{
	ifstream fin("resource/config/levels/levelPaths.txt", std::ios::out);

	if (fin.is_open())
	{
		unsigned int count = 0;
		for (unsigned int count = 0; !fin.eof() ; count++)
		{
			getline(fin, levels[count]);
		}
	}
}
