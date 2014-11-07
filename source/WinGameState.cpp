#define NUM_CHOICES		2

#include "WinGameState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "CreditsState.h"


/**************************************************************/
// GetInstance
/*static*/ WinGameState* WinGameState::GetInstance( void )
{
	static WinGameState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void WinGameState::Enter( void )
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {0, 0, 0} );	// black


	// Load assets
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/youWin.png");
}


/**************************************************************/
// Exit
/*virtual*/ void WinGameState::Exit( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);
}


/**************************************************************/
// Input
/*virtual*/ bool WinGameState::Input( void )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
		m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
		m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;


	if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonPressed(0, 1) == true)
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
				Game::GetInstance()->AddState(CreditsState::GetInstance());
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
/*virtual*/ void WinGameState::Update( float elapsedTime )
{
}


/**************************************************************/
// Render
/*virtual*/ void WinGameState::Render( void )
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();


	// Draw the background image
	pGraphics->DrawTexture(m_hBackgroundImage, { 0, -60 });


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width		= Game::GetInstance()->GetScreenWidth();
	float height	= Game::GetInstance()->GetScreenHeight();
	float scale		= 0.9f;
	float left_start = 50.0F;


	// Display the text centered
	//pFont->Draw("YOU WIN!", { (width - (8 * 32 * 3.0f)) / 2, height * 0.25F - (26.0F * 3.0F) }, 3.0f, { 255, 255, 255 });


	pFont->Draw("Play Again?", { (width - (11 * 25 * scale)) / 2, height * 0.5F }, scale, { 255, 255, 0 });
	pFont->Draw("YES", { (width - (3 * 32 * scale)) / 2, (height * 0.5F) + 100.0f }, scale, { 0, 255, 0 });
	pFont->Draw("NO", { (width - (2 * 32 * scale)) / 2, (height * 0.5F) + 200.0f }, scale, { 0, 255, 0 });


	const char*	output = m_nCursor == 0 ? "=    =" : "=   =";
	int			length = m_nCursor == 0 ? 3 : 2;
	pFont->Draw(output, { (width - (length * 32)) / 2 - 32.0F, (height * 0.5F) + (100.0f * m_nCursor) + 100.0f }, 1.0f, { 255, 0, 0 });
}
