#define NUM_CHOICES		4

#include "PauseState.h"

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
/*static*/ PauseState* PauseState::GetInstance(void)
{
	static PauseState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void PauseState::Enter(void)
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	m_nCursor = 0;


	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black


	// Load assets
}

/**************************************************************/
// Exit
/*virtual*/ void PauseState::Exit(void)
{
	// Unload assets


}


/**************************************************************/
// Input
/*virtual*/ bool PauseState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsButtonPressed(0, 2) == true)
	{
		SGD::Event msg("UNPAUSE");
		msg.SendEventNow();
		Game::GetInstance()->RemoveState();

		return true;
	}


	if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
		m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
		m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;


	if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonDown(0, 1) == true)
	{

		switch (m_nCursor)
		{
		case 0: // gameplay
		{
					SGD::Event msg("UNPAUSE");
					msg.SendEventNow();
					Game::GetInstance()->RemoveState();

					return true;
		}
			break;

		case 1: // restart from checkpoint
		{
					
					return true;
		}
			break;
		case 2: // restart level
			
		{
					//Game::GetInstance()->RemoveState();
					//Game::GetInstance()->RemoveState();
					//Game::GetInstance()->AddState(GameplayState::GetInstance());
					return true;
		}
			break;
		case 3: // main menu
		{
					Game::GetInstance()->RemoveState();
					Game::GetInstance()->RemoveState();
					Game::GetInstance()->AddState(MainMenuState::GetInstance());
					return true;
		}
			break;
		}
	}


	return true;	// keep playing
}


/**************************************************************/
// Update
/*virtual*/ void PauseState::Update(float elapsedTime)
{
	
}


/**************************************************************/
// Render
/*virtual*/ void PauseState::Render(void)
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	GameplayState::GetInstance()->Render();
	pGraphics->DrawRectangle({ 0, 0, 1024, 768 }, { 210, 0, 0, 0 });


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();
	float scale = 0.9f;
	float left_start = 50.0F;											// 50


	// Display the game title centered at 4x scale
	pFont->Draw("PAUSED", { (width - (9 * 32 * 3.0f)) / 2, (26.0F * 3.0F) }, 3.0f, { 255, 255, 255 });



	SGD::Point position;

	//position.x	= (width * 0.5F) - 30.0F * scale;
	//position.y	= (height * 0.5F) - 30.0F * scale;

	/*
	if (m_pLoseAnimation != nullptr && m_pLoseAnimation->IsPlaying() == true)
	m_pLoseAnimation->Render(position, scale, { 255, 255, 255, 255 });
	*/


	//pFont->Draw("?", { (width - (9 * 32 * scale)) / 2, height * 0.5F }, scale, { 255, 255, 0 });

	pFont->Draw("Resume", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 100.0f }, scale, { 0, 255, 0 });

	pFont->Draw("Restart from Checkpoint", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 200.0f }, scale, { 0, 255, 0 });

	pFont->Draw("Restart from Beginning", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 300.0f }, scale, { 0, 255, 0 });


	pFont->Draw("Quit to Menu", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 0, 255, 0 });




	const char* output = "=";

	if (m_nCursor == 0)
	{
		
		position.x = width*0.05f;
		position.y = (height * 0.25F) + 100.0f;
	}
	else if (m_nCursor == 1)
	{
		
		position.x = width*0.05f;
		position.y = (height * 0.25F) + 200.0f;
	}
	else if (m_nCursor == 2)
	{
		
		position.x = width*0.05f;
		position.y = (height * 0.25F) + 300.0f;
	}

	else if (m_nCursor == 3)
	{
		
		position.x = width*0.05f ;
		position.y = (height * 0.25F) + 400.0f;
	}



	pFont->Draw(output, position, 1.0f, { 255, 0, 0 });


}
