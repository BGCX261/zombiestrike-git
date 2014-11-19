#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include <vector>
#include <map>
using namespace std;

struct Tile
{
	SGD::Point renderPos;
	SGD::Point worldPos;



};

struct Layer
{
	//vector<Tile> m_vTiles;
	Tile** m_vTiles = nullptr;



};



class Map
{
	//SGD::Point tilePos;
protected:

	SGD::HTexture m_hPalleteTexture;
	vector<Layer> mapLayers;
	
	string m_szTilePath;

	float m_nMapWidth = 0.0f;
	float m_nMapHeight = 0.0f;
	SGD::Size tileSize;
	
public:

	Map();
	~Map();

	//Accessors
	SGD::HTexture GetTexture() { return m_hPalleteTexture; }
	string GetTilePath() { return m_szTilePath; }
	float GetMapWidth() { return m_nMapWidth; }
	float GetMapHeight() { return m_nMapHeight; }

	//Mutators
	void SetTexture(SGD::HTexture bgImage) { m_hPalleteTexture = bgImage; }
	void SetTilePath(string path)	{ m_szTilePath = path; }
	void SetMapWidth(float mapW) { m_nMapWidth = mapW; }
	void SetMapHeight(float mapH) { m_nMapHeight = mapH; }

	map<SGD::Point, Tile> tileMap;
};

