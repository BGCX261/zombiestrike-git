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
#include "HowToPlayState.h"
#include "OptionsState.h"
#include "HTPGameState.h"
#include "IntroState.h"


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

	int temp = 0;
	temp++;

	if (temp > 0)
	{
		temp++;
	}
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

	if (HTPGameState::GetInstance()->GetChoiceScreen() == true)
	{
		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;
	}
	else
	{
		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < (NUM_CHOICES + 1) ? m_nCursor + 1 : 0;
		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : (NUM_CHOICES + 1) - 1;
	}


	if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonPressed(0, 1) == true)
	{
		/*
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

		case 1: // controls
		{
					Game::GetInstance()->AddState(HowToPlayState::GetInstance());
					return true;
		}
			break;
		case 2: // options
			
		{
					//Game::GetInstance()->RemoveState();
					//Game::GetInstance()->RemoveState();
					//Game::GetInstance()->AddState(GameplayState::GetInstance());
					Game::GetInstance()->AddState(OptionsState::GetInstance());
					return true;
		}
			break;
		case 3: // main menu
		{
					if (HTPGameState::GetInstance()->GetChoiceScreen() == true)
					{
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->AddState(MainMenuState::GetInstance());
						return true;
					}

					else
					{
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->AddState(IntroState::GetInstance());
						return true;
					}
		}
			break;
		}
		*/


		if (HTPGameState::GetInstance()->GetChoiceScreen() == true)
		{
			#pragma region Gameplay

			switch (m_nCursor)
			{
			case 0: // resume gameplay
			{
						SGD::Event msg("UNPAUSE");
						msg.SendEventNow();
						Game::GetInstance()->RemoveState();
						return true;
			}
				break;

			case 1: // controls
			{
						Game::GetInstance()->AddState(HowToPlayState::GetInstance());
						return true;
			}
				break;

			case 2: // options
			{
						Game::GetInstance()->AddState(OptionsState::GetInstance());
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

			default:
				break;
			}

#pragma endregion
		}
		else
		{
			#pragma region Tutorial

			switch (m_nCursor)
			{
			case 0: // resume tutorial
			{
						SGD::Event msg("UNPAUSE");
						msg.SendEventNow();
						Game::GetInstance()->RemoveState();
						return true;
			}
				break;

			case 1: // gameplay
			{
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->AddState(IntroState::GetInstance());
						return true;
			}
				break;

			case 2: // controls
			{
						Game::GetInstance()->AddState(HowToPlayState::GetInstance());
						return true;
			}
				break;

			case 3: // options
			{
						Game::GetInstance()->AddState(OptionsState::GetInstance());
						return true;
			}
				break;

			case 4: // main menu
			{
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->RemoveState();
						Game::GetInstance()->AddState(MainMenuState::GetInstance());
						return true;
			}
				break;

			default:
				break;
			}

#pragma endregion
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
	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();

	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	if (HTPGameState::GetInstance()->GetChoiceScreen() == false)
		HTPGameState::GetInstance()->Render();
	else
		GameplayState::GetInstance()->Render();

	pGraphics->DrawRectangle({ 0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() }, { 210, 0, 0, 0 });

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	
	float scale = 1.25f;
												


	// Display the game title centered at 4x scale
	pFont->Draw("PAUSED", { (width - (9 * 32 * 3.0f)) / 2, (26.0F * 3.0F) }, 3.0f, { 255, 255, 255 });


	/*
	if (m_nCursor == 0)
	{
		pFont->Draw("Resume", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 100.0f }, scale, { 255,255,255 });
		pFont->Draw("Controls", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 200.0f }, scale, { 255, 0, 0 });
		pFont->Draw("Options", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 300.0f }, scale, { 255, 0, 0 });

		if (HTPGameState::GetInstance()->GetChoiceScreen() == false)
			pFont->Draw("Start Game", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 0, 0 });
		else
			pFont->Draw("Quit to Menu", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 0, 0 });
	}
	else if (m_nCursor == 1)
	{
		pFont->Draw("Resume", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 100.0f }, scale, { 255, 0, 0 });
		pFont->Draw("Controls", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 200.0f }, scale, { 255, 255, 255 });
		pFont->Draw("Options", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 300.0f }, scale, { 255, 0, 0 });

		if (HTPGameState::GetInstance()->GetChoiceScreen() == false)
			pFont->Draw("Start Game", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 0, 0 });
		else
			pFont->Draw("Quit to Menu", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 0, 0 });
	}
	else if (m_nCursor == 2)
	{
		pFont->Draw("Resume", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 100.0f }, scale, { 255, 0, 0 });
		pFont->Draw("Controls", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 200.0f }, scale, { 255, 0, 0 });
		pFont->Draw("Options", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 300.0f }, scale, { 255, 255, 255 });

		if (HTPGameState::GetInstance()->GetChoiceScreen() == false)
			pFont->Draw("Start Game", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 0, 0 });
		else
			pFont->Draw("Quit to Menu", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 0, 0 });
	}
	else if (m_nCursor == 3)
	{
		pFont->Draw("Resume", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 100.0f }, scale, { 255, 0, 0 });
		pFont->Draw("Controls", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 200.0f }, scale, { 255, 0, 0 });
		pFont->Draw("Options", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 300.0f }, scale, { 255, 0, 0 });

		if (HTPGameState::GetInstance()->GetChoiceScreen() == false)
			pFont->Draw("Start Game", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 255, 255 });
		else
			pFont->Draw("Quit to Menu", { (width*0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + 400.0f }, scale, { 255, 255, 255 });
	}
	*/


	// during gameplay
	if (HTPGameState::GetInstance()->GetChoiceScreen() == true)
	{
		string gameplaychoices[4] = { "Resume", "Controls", "Options", "Quit to Menu" };
		for (size_t i = 0; i < 4; i++)
		{
			SGD::Color color = m_nCursor == i
				? SGD::Color(255, 255, 255)
				: SGD::Color(255, 0, 0);

			float y_offset = (100.0f * i) + 100.0f;
			pFont->Draw(gameplaychoices[i].c_str(), { (width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + y_offset }, scale, color);
		}
	}


	// during tutorial
	else
	{
		string tutorialchoices[5] = { "Resume", "Start Game", "Controls", "Options", "Quit to Menu" };
		for (size_t i = 0; i < 5; i++)
		{
			SGD::Color color = m_nCursor == i
				? SGD::Color(255, 255, 255)
				: SGD::Color(255, 0, 0);

			float y_offset = (100.0f * i) + 100.0f;
			pFont->Draw(tutorialchoices[i].c_str(), { (width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + y_offset }, scale, color);
		}
	}

}
