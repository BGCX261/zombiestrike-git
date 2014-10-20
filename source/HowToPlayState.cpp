#include "HowToPlayState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"


/**************************************************************/
// GetInstance
/*static*/ HowToPlayState* HowToPlayState::GetInstance( void )
{
	static HowToPlayState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void HowToPlayState::Enter( void )
{
}


/**************************************************************/
// Exit
/*virtual*/ void HowToPlayState::Exit( void )
{
}


/**************************************************************/
// Input
/*virtual*/ bool HowToPlayState::Input( void )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonDown(0, 2) == true)
		Game::GetInstance()->RemoveState();


	return true;
}


/**************************************************************/
// Update
/*virtual*/ void HowToPlayState::Update( float elapsedTime )
{
}


/**************************************************************/
// Render
/*virtual*/ void HowToPlayState::Render( void )
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


	// Display the title centered at 4x scale
	pFont->Draw("How To Play", { (width - (11 * 32 * 2.5f)) / 2, 10 }, 2.5f, { 255, 255, 255 });


	// Align text based on window width
	float left_start	= 10.0F;
	float starting_y	= 110.0F;

	std::string objective	= "Objective:\n pppppp\n\n\n";
	std::string controls	= "Controls:\n\nW\t\tMove towards mouse\nS\t\tMove away from mouse\nA\t\tStrafe left\nS\t\tStrafe right\n\n";
	std::string skills		= "Space\tToggle Active Camo\n\n\n";
	
	std::string output = objective + controls + skills;

	pFont->Draw(output.c_str(), { left_start, starting_y }, 0.8f, { 0, 255, 0 });
}
