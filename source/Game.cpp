#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "LoseGameState.h"

#include <ctime>
#include <cstdlib>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlobj.h>
#include <fstream>


/**************************************************************/
// Singleton
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance
//	- allocate the ONE instance & return it
/*static*/ Game* Game::GetInstance( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the ONE instance
/*static*/ void Game::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/**************************************************************/
// Initialize
//	- initialize the SGD wrappers
//	- enter the Main Menu state
bool Game::Initialize( float width, float height, const wchar_t* title )
{
	// Seed First!
	srand( (unsigned int)time( nullptr ) );
	rand();

	
	// Initialize the wrappers
	if( SGD::AudioManager::GetInstance()->Initialize() == false 
		|| SGD::GraphicsManager::GetInstance()->Initialize( false ) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false )
		return false;


	// Store the size parameters
	m_fScreenWidth	= width;
	m_fScreenHeight = height;


	// Store the title parameter
	m_strGameTitle = title;


	// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize("resource/bitmapfonts/MetalGearFont.xml", '\0', false);


	// Load assets
	loadScreen = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Loading.png");


	// Setup the profiles
	CreateProfiles();
	LoadProfiles();


	// Start the game in the Main Menu state
	m_pCurrState = MainMenuState::GetInstance();
	m_pCurrState->Enter();
	stateMachine.push(m_pCurrState);


	// Store the current time (in milliseconds)
	m_ulGameTime	= GetTickCount();


	return true;	// success!
}


/**************************************************************/
// Main
//	- update the SGD wrappers
//	- run the current state
int Game::Update( void )
{
	// Update the wrappers
	if( SGD::AudioManager::GetInstance()->Update() == false 
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false )
		return -10;		// exit FAILURE!

	
	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();					// current time in milliseconds
	elapsedTime = (now - m_ulGameTime) / 1000.0f;		// convert to fraction of a second
	m_ulGameTime = now;									// store the current time

	// Cap the elapsed time to 1/8th of a second
	if( elapsedTime > 0.125f )
		elapsedTime = 0.125f;


	/**********************************************************/
	// 'Alt + Enter' -> Full screen
	/**********************************************************/

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Let the current state handle input
	IGameState* pCurrent = stateMachine.top();
	if (pCurrent->Input() == false)
		return 1;	// exit success!


	// Update & render the current state if it was not changed
	if (pCurrent == stateMachine.top())
	{
		pCurrent->Update(elapsedTime);
		pCurrent->Render();
	}


	return 0;		// keep playing!
}


/**************************************************************/
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate( void )
{
	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;


	// Exit the current state
	IGameState* currState = stateMachine.top();
	currState->Exit();
	currState = nullptr;
	stateMachine.pop();


	// Unload assets
	SGD::GraphicsManager::GetInstance()->UnloadTexture(loadScreen);

	
	// Terminate the core SGD wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
}


/**************************************************************/
// AddState
//	- enter the new state to allocate resources
void Game::AddState( IGameState* pNewState )
{
	// Enter the new state
	m_pCurrState = pNewState;
	m_pCurrState->Enter();
	

	// Store the new state
	stateMachine.push(m_pCurrState);
}


/**************************************************************/
// RemoveState
//	- exit the current state to release resources
void Game::RemoveState( void )
{
	// Exit the old state
	m_pCurrState = stateMachine.top();
	if (m_pCurrState != nullptr)
		m_pCurrState->Exit();

	stateMachine.pop();
}


/**************************************************************/
// LoadProfiles
void Game::LoadProfiles( void )
{
	//* If using Wide characters under project settings *//
	HRESULT hr;
	ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;


	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &wszPath);


	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);


	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	string pathtowrite = stringstream.str();


	// Add the company and game information
	pathtowrite += "\\GoldNuggetGames\\SilentStrike\\";


	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);


	// Create our save file
	for (unsigned int i = 1; i < 4; i++)
	{
		string filePath = pathtowrite;

		filePath += "savefile.save";
		filePath += std::to_string(i);

		ifstream fin(filePath.c_str());
		if (fin.is_open())
		{
			getline(fin, profiles[i - 1].path);
			fin >> profiles[i - 1].MaxEnergy;
			fin >> profiles[i - 1].MaxStamina;
			fin >> profiles[i - 1].CamoMultiplier;
			fin >> profiles[i - 1].SpeedMultiplier;
			fin >> profiles[i - 1].RadarMultiplier;
			fin >> profiles[i - 1].LevelsComplete;
			fin >> profiles[i - 1].m_bHasKey;
			fin >> profiles[i - 1].CheckPointReached;
			fin.close();
		}
	}
}


/**************************************************************/
// CreateProfiles
void Game::CreateProfiles()
{
	//* If using Wide characters under project settings *//
	HRESULT hr;
	ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;


	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &wszPath);


	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);


	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	string pathtowrite = stringstream.str();


	// Add the company and game information
	pathtowrite += "\\GoldNuggetGames\\SilentStrike\\";


	// Create our directory
	if (SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0) == ERROR_SUCCESS)
	{
		// Create our save file
		for (unsigned int i = 1; i < 4; i++)
		{
			string filePath = pathtowrite;

			filePath += "savefile.save";
			filePath += std::to_string(i);

			fstream fout(filePath.c_str(), std::ios_base::out);
			if (fout.is_open())
			{
				fout << filePath << '\n';

				fout << profiles[i - 1].MaxEnergy << '\n';
				fout << profiles[i - 1].MaxStamina << '\n';
				fout << profiles[i - 1].CamoMultiplier << '\n';
				fout << profiles[i - 1].SpeedMultiplier << '\n';
				fout << profiles[i - 1].RadarMultiplier << '\n';
				fout << profiles[i - 1].LevelsComplete << '\n';
				fout << profiles[i - 1].m_bHasKey << '\n';
				fout << profiles[i - 1].CheckPointReached;
				fout.close();
			}
		}
	}
}