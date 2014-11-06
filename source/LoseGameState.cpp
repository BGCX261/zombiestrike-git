#define NUM_CHOICES		2

#include "LoseGameState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"
#include "GameplayState.h"


/**************************************************************/
// GetInstance
/*static*/ LoseGameState* LoseGameState::GetInstance( void )
{
	static LoseGameState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void LoseGameState::Enter( void )
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {0, 0, 0} );	// black
	m_nCursor = 0;

	// Load assets
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/youLose.png");
}


/**************************************************************/
// Exit
/*virtual*/ void LoseGameState::Exit( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);

}


/**************************************************************/
// Input
/*virtual*/ bool LoseGameState::Input( void )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	if (pInput->IsKeyPressed(SGD::Key::Down) == true)
		m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true)
		m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;


	if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{
		switch (m_nCursor)
		{
		case 0: // gameplay
			{
				SGD::Event msg("UNPAUSE");
				msg.SendEventNow();
				Game::GetInstance()->RemoveState();
				Game::GetInstance()->RemoveState();
				Game::GetInstance()->AddState(GameplayState::GetInstance());
				return true;
			}
			break;

		case 1: // main menu
			{
				Game::GetInstance()->RemoveState();
				Game::GetInstance()->RemoveState();
				Game::GetInstance()->AddState(MainMenuState::GetInstance());
				return true;
			}
			break;
		}
	}


	// keep playing
	return true;
}


/**************************************************************/
// Update
/*virtual*/ void LoseGameState::Update( float elapsedTime )
{
}


/**************************************************************/
// Render
/*virtual*/ void LoseGameState::Render( void )
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();


	// Draw the background image
	pGraphics->DrawTexture(m_hBackgroundImage, { -10, -190 });


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width		= Game::GetInstance()->GetScreenWidth();
	float height	= Game::GetInstance()->GetScreenHeight();
	float scale		= 0.9f;
	float left_start = 50.0F;


	// Display the text centered
	//pFont->Draw("YOU LOSE!", { (width - (9 * 32 * 3.0f)) / 2, height * 0.25F - (26.0F * 3.0F) }, 3.0f, { 255, 255, 255 });
	//pFont->Draw("GAME OVER!", { (width - (10 * 32 * 3.0f)) / 2, height * 0.25F - (26.0F * 3.0F) }, 3.0f, { 255, 255, 255 });


	pFont->Draw("Continue?", { (width - (9 * 24 * scale)) / 2, height * 0.53F }, scale, { 255, 255, 0 });
	pFont->Draw("YES", { (width - (3 * 32 * scale)) / 2, (height * 0.5F) + 100.0f }, scale, { 0, 255, 0 });
	pFont->Draw("NO", { (width - (2 * 32 * scale)) / 2, (height * 0.5F) + 200.0f }, scale, { 0, 255, 0 });


	const char*	output	= m_nCursor == 0 ? "=    =" : "=   =";
	int			length	= m_nCursor == 0 ? 3 : 2;
	pFont->Draw(output, { (width - (length * 32)) / 2 - 32.0F, (height * 0.5F) + (100.0f * m_nCursor) + 100.0f }, 1.0f, { 255, 0, 0 });
}
