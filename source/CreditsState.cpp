#define SCROLL_OFFSET		50.7F//47.0F

#include "CreditsState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"


/**************************************************************/
// GetInstance
/*static*/ CreditsState* CreditsState::GetInstance( void )
{
	static CreditsState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void CreditsState::Enter( void )
{	
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {0, 0, 0} );	// black
}

/**************************************************************/
// Exit
/*virtual*/ void CreditsState::Exit( void )
{
}


/**************************************************************/
// Input
/*virtual*/ bool CreditsState::Input( void )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonDown(0, 2) == true)
		Game::GetInstance()->RemoveState();


	return true;
}


/**************************************************************/
// Update
/*virtual*/ void CreditsState::Update( float elapsedTime )
{
}


/**************************************************************/
// Render
/*virtual*/ void CreditsState::Render( void )
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


	// Display the title centered at 4x scale
	pFont->Draw("CREDITS", { (width - (7 * 32 * 3.0f)) / 2, 10 }, 3.0f, { 255, 255, 255 });


	// Align text based on window width
	float left_start	= 10.0F;

	std::string developers	= "Developers: Gold Nugget Games\n\tConner McLaine\n\tJoshua Griggs\n\tJason  Bloomfield\n\n";
	std::string school		= "School:\n\tFull Sail University\n\n";
	std::string fsclass		= "Class:\n\tStructure of Game Production\n\n";
	std::string artists		= "Artists:\n\tCaris   Frazier\n\tGregory Bay\n\n";
	std::string aps			= "Associate Producers:\n\tRobert Martinez\n\tSean   Hathaway\n\tShawn  Paris";
	
	std::string output = developers + school + fsclass + artists + aps;

	pFont->Draw(output.c_str(), { left_start, 140.0f }, 0.8f, { 255, 0, 0 });
}
