#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "WeaponManager.h"

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

	ShowCursor(false);
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

	m_hHudWpn = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/hudweapons.png");

	m_hWpnSwitch = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/switchweapon.wav");

	// Load assets
	loadScreen = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Loading.png");


	// Setup the profiles
	CreateStoryProfiles();
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
	m_pCurrState = stateMachine.top();
	if (m_pCurrState->Input() == false)
		return 1;	// exit success!


	// Update & render the current state if it was not changed
	if (m_pCurrState == stateMachine.top())
	{
		m_pCurrState->Update(elapsedTime);

		if (m_pCurrState != nullptr)
		{
			m_pCurrState->Render();
		}
		
	}


	return 0;		// keep playing!
}


/**************************************************************/
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate( void )
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

	pGraphics->UnloadTexture(m_hHudWpn);
	pAudio->UnloadAudio(m_hWpnSwitch);

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

	m_pCurrState = nullptr;

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
	pathtowrite += "\\CTS\\ZombieStrike\\Story\\";


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

#pragma region Pistols

			//pistol
			fin >> profiles[i - 1].pistol.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].pistol.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].pistol.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pistol.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].pistol.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].pistol.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pistol.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].pistol.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].pistol.reloadTime.upgradedSkill.maxTier;

			//revolver
			fin >> profiles[i - 1].revolver.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.penPower.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.penPower.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.penPower.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].revolver.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].revolver.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].revolver.isBought;


#pragma endregion





#pragma region SMGs
			//Mac10
			fin >> profiles[i - 1].mac10.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].mac10.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].mac10.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].mac10.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].mac10.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].mac10.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].mac10.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].mac10.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].mac10.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].mac10.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].mac10.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].mac10.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].mac10.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].mac10.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].mac10.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].mac10.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].mac10.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].mac10.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].mac10.isBought;
		
			//Tech9
			fin >> profiles[i - 1].tech9.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].tech9.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].tech9.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].tech9.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].tech9.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].tech9.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].tech9.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].tech9.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].tech9.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].tech9.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].tech9.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].tech9.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].tech9.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].tech9.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].tech9.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].tech9.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].tech9.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].tech9.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].tech9.isBought;

			//P90
			fin >> profiles[i - 1].p90.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].p90.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].p90.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].p90.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].p90.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].p90.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].p90.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].p90.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].p90.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].p90.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].p90.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].p90.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].p90.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].p90.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].p90.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].p90.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].p90.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].p90.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].p90.isBought;

#pragma endregion

#pragma region Shotguns

			//SawnOff
			fin >> profiles[i - 1].sawnoff.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].sawnoff.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].sawnoff.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sawnoff.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].sawnoff.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].sawnoff.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sawnoff.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].sawnoff.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].sawnoff.reloadTime.upgradedSkill.maxTier;


			fin >> profiles[i - 1].sawnoff.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].sawnoff.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].sawnoff.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sawnoff.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].sawnoff.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].sawnoff.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sawnoff.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].sawnoff.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].sawnoff.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sawnoff.isBought;


			//Pump
			fin >> profiles[i - 1].pumpShotgun.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].pumpShotgun.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].pumpShotgun.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].pumpShotgun.isBought;

			//Auto
			fin >> profiles[i - 1].autoShotgun.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].autoShotgun.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].autoShotgun.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].autoShotgun.isBought;



#pragma endregion

#pragma region Assault Rifles

			//AK-47
			fin >> profiles[i - 1].ak47.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].ak47.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].ak47.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].ak47.isBought;

			//M-16
			fin >> profiles[i - 1].m16.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].m16.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].m16.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].m16.isBought;

			//LMG
			fin >> profiles[i - 1].lmg.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].lmg.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].lmg.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].lmg.isBought;

#pragma endregion

#pragma region Heavy Weapons

			//Sniper
			fin >> profiles[i - 1].sniper.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.penPower.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.penPower.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.penPower.upgradedSkill.maxTier;


			fin >> profiles[i - 1].sniper.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.bulletSpread.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.recoilTime.upgradedSkill.stat;
			fin >> profiles[i - 1].sniper.recoilTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].sniper.recoilTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].sniper.isBought;

			//Flamethrower

			fin >> profiles[i - 1].flameThrower.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].flameThrower.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].flameThrower.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].flameThrower.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].flameThrower.bulletSpread.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.bulletSpread.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.bulletSpread.upgradedSkill.maxTier;
		
			fin >> profiles[i - 1].flameThrower.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].flameThrower.bulletVelocity.upgradedSkill.stat;
			fin >> profiles[i - 1].flameThrower.bulletVelocity.upgradedSkill.currTier;
			fin >> profiles[i - 1].flameThrower.bulletVelocity.upgradedSkill.maxTier;

			fin >> profiles[i - 1].flameThrower.isBought;

			//Grenade Launcher

			fin >> profiles[i - 1].nadeLauncher.totalAmmo.upgradedSkill.stat;
			fin >> profiles[i - 1].nadeLauncher.totalAmmo.upgradedSkill.currTier;
			fin >> profiles[i - 1].nadeLauncher.totalAmmo.upgradedSkill.maxTier;

			fin >> profiles[i - 1].nadeLauncher.magSize.upgradedSkill.stat;
			fin >> profiles[i - 1].nadeLauncher.magSize.upgradedSkill.currTier;
			fin >> profiles[i - 1].nadeLauncher.magSize.upgradedSkill.maxTier;

			fin >> profiles[i - 1].nadeLauncher.ammoCap.upgradedSkill.stat;
			fin >> profiles[i - 1].nadeLauncher.ammoCap.upgradedSkill.currTier;
			fin >> profiles[i - 1].nadeLauncher.ammoCap.upgradedSkill.maxTier;

			fin >> profiles[i - 1].nadeLauncher.reloadTime.upgradedSkill.stat;
			fin >> profiles[i - 1].nadeLauncher.reloadTime.upgradedSkill.currTier;
			fin >> profiles[i - 1].nadeLauncher.reloadTime.upgradedSkill.maxTier;

			fin >> profiles[i - 1].nadeLauncher.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].nadeLauncher.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].nadeLauncher.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].nadeLauncher.bulletVelocity.upgradedSkill.stat;
			fin >> profiles[i - 1].nadeLauncher.bulletVelocity.upgradedSkill.currTier;
			fin >> profiles[i - 1].nadeLauncher.bulletVelocity.upgradedSkill.maxTier;
		
			fin >> profiles[i - 1].nadeLauncher.isBought;

			//Barb Wire
			fin >> profiles[i - 1].barbWire.maxHealth.upgradedSkill.stat;
			fin >> profiles[i - 1].barbWire.maxHealth.upgradedSkill.currTier;
			fin >> profiles[i - 1].barbWire.maxHealth.upgradedSkill.maxTier;

			fin >> profiles[i - 1].barbWire.damage.upgradedSkill.stat;
			fin >> profiles[i - 1].barbWire.damage.upgradedSkill.currTier;
			fin >> profiles[i - 1].barbWire.damage.upgradedSkill.maxTier;

			fin >> profiles[i - 1].barbWire.isBought;
			bool temp;
			for (size_t j = 0; j < 30; j++)
			{
				
				fin >> profiles[i - 1].barbWireStates[j];
				

			}
			//Sandbag

			fin >> profiles[i - 1].sandBag.maxHealth.upgradedSkill.stat;
			fin >> profiles[i - 1].sandBag.maxHealth.upgradedSkill.currTier;
			fin >> profiles[i - 1].sandBag.maxHealth.upgradedSkill.maxTier;


			fin >> profiles[i - 1].sandBag.isBought;

			for (size_t j = 0; j < 30; j++)
			{
				
				fin >> profiles[i - 1].sandBagStates[j];
			

			}
			

			fin >> profiles[i - 1].landMine.isBought;

			for (size_t j = 0; j < 50; j++)
			{
				
				fin >> profiles[i - 1].landMineStates[j];
				

			}

			fin >> profiles[i - 1].numTurrets;




			//LandMine



#pragma endregion
			
			fin.close();
		}
	}
}


/**************************************************************/
// CreateProfiles
void Game::CreateStoryProfiles()
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
	pathtowrite += "\\CTS\\ZombieStrike\\Story\\";


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
				WeaponManager::GetInstance()->GetWeapons();
				fout << filePath << '\n';

#pragma region Pistols

				//pistol stats
				fout << 10 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 2 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .33 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				//revolver
				fout << 25 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 25 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 50 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
#pragma endregion

#pragma region SMGs
				//Mac10
				fout << 60 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 20 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 60 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 2 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 20 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//Tech9
				fout << 90 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 30 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 90 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 35 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//p90
				fout << 150 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 50 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 150 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 35 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';

#pragma endregion

#pragma region Shotguns
				//sawn off
				fout << 16 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 16 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 2 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 20 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 35 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//pump
				fout << 24 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 6 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 24 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 4 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 10 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 25 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//auto shotty
				fout << 30 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 8 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 30 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 15 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 25 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';

#pragma endregion


#pragma region Assault Rifles
				//ak47
				fout << 90 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 30 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 90 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 7 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 40 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .33 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//m16
				fout << 90 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 30 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 90 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 25 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .2 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n'; 
				//LMG
				fout << 200 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 100 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 200 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 6 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 75 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << .33 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';

#pragma endregion


#pragma region Heavy Weapons
				//sniper
				fout << 15 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 2 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 15 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 2 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 200 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//flamethrower
				fout << 200 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 100 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 200 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 4 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 10 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 20 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 300 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';
				//grenade launcher
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 1 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 5 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 4 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 200 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 300 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';
				fout << 0 << '\n';

				//barbedwire
				fout << 100 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';

				fout << 10 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';

				fout << 0 << '\n';

				for (size_t i = 0; i < 30; i++)
				{
					fout << 0 << '\n';
				}

				//sandbags
				fout << 250 << '\n';
				fout << 1 << '\n';
				fout << 3 << '\n';

				fout << 0 << '\n';

				for (size_t i = 0; i < 30; i++)
				{
					fout << 0 << '\n';
				}

				//landmines
				fout << 0 << '\n';

				for (size_t i = 0; i < 50; i++)
				{
					fout << 0 << '\n';
				}


				//turrets

				fout << 0 << '\n';

#pragma endregion

				fout.close();
			}
		}
	}
}
void Game::CreateSurvivalProfiles()
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
	pathtowrite += "\\CTS\\ZombieStrike\\Survival\\";


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

				/*fout << profiles[i - 1].MaxEnergy << '\n';
				fout << profiles[i - 1].MaxStamina << '\n';
				fout << profiles[i - 1].CamoMultiplier << '\n';
				fout << profiles[i - 1].SpeedMultiplier << '\n';
				fout << profiles[i - 1].RadarMultiplier << '\n';
				fout << profiles[i - 1].LevelsComplete << '\n';
				fout << profiles[i - 1].m_bHasKey << '\n';
				fout << profiles[i - 1].CheckPointReached;*/
				fout.close();
			}
		}
	}
}