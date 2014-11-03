#include "IntroState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"


/**************************************************************/
// GetInstance
/*static*/ IntroState* IntroState::GetInstance( void )
{
	static IntroState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void IntroState::Enter( void )
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {0, 0, 0} );	// black


	// Load assets
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	//m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/youLose.png");
}


/**************************************************************/
// Exit
/*virtual*/ void IntroState::Exit( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// Unload assets
	//pGraphics->UnloadTexture(m_hBackgroundImage);

}


/**************************************************************/
// Input
/*virtual*/ bool IntroState::Input( void )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		Game::GetInstance()->RemoveState();
		Game::GetInstance()->AddState(GameplayState::GetInstance());
		return true;
	}



	// keep playing
	return true;
}


/**************************************************************/
// Update
/*virtual*/ void IntroState::Update( float elapsedTime )
{
}


/**************************************************************/
// Render
/*virtual*/ void IntroState::Render( void )
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();


	// Draw the background image
	//pGraphics->DrawTexture(m_hBackgroundImage, { -10, -190 });


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

}
