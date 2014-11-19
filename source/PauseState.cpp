#define NUM_CHOICES		4

#include "PauseState.h"
#include "ShopState.h"

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
	m_hReticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/MenuImages/Reticle3.png", { 0, 0, 0 });



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
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hReticleImage);
}


/**************************************************************/
// Input
/*virtual*/ bool PauseState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();
	float scale = 1.25f;
	/*if (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0)
	{
		SGD::Point	mpoint = pInput->GetMousePosition();
		SGD::Vector	joystick = pInput->GetLeftJoystick(0);
		float		stickmin = 0.250f;
		float		mousevel = 1.0f;


		if (joystick.x > stickmin)
			mpoint.x += mousevel;
		else if (joystick.x < stickmin * -1.0f)
			mpoint.x -= mousevel;

		if (joystick.y > stickmin)
			mpoint.y += mousevel;
		else if (joystick.y < stickmin * -1.0f)
			mpoint.y -= mousevel;

		if (mpoint.x < 0.0F)
			mpoint.x = 0.0F;
		if (mpoint.y < 0.0F)
			mpoint.y = 0.0F;
		if (mpoint.x > Game::GetInstance()->GetScreenWidth())
			mpoint.x = Game::GetInstance()->GetScreenWidth();
		if (mpoint.y > Game::GetInstance()->GetScreenHeight())
			mpoint.y = Game::GetInstance()->GetScreenHeight();

		pInput->SetMousePosition(mpoint);
	}*/

	SGD::Point mousePos = pInput->GetMousePosition();
	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsButtonPressed(0, 2) == true)
	{
		SGD::Event msg("UNPAUSE");
		msg.SendEventNow();
		Game::GetInstance()->RemoveState();

		return true;
	}
	if (pInput->GetMouseMovement() != SGD::Vector() || (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0))
	{
		

		if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + (100.0f * 0) + 100.0f), SGD::Size(256, 64))))
			m_nCursor = 0;
		else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + (100.0f * 1) + 100.0f), SGD::Size(256, 64))))
			m_nCursor = 1;
		else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + (100.0f * 2) + 100.0f), SGD::Size(256, 64))))
			m_nCursor = 2;
		else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + (100.0f * 3) + 100.0f), SGD::Size(256, 64))))
			m_nCursor = 3;
		if (HTPGameState::GetInstance()->GetIsCurrState() == true)
		{
			if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width * 0.25f - (2 * 32 * scale)) / 2, (height * 0.25F) + (100.0f * 4) + 100.0f), SGD::Size(256, 64))))
				m_nCursor = 4;

		}
		

	}

	if (HTPGameState::GetInstance()->GetIsCurrState() == false)
	{
		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;
	}
	else if (HTPGameState::GetInstance()->GetIsCurrState() == false)
	{
		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsKeyPressed(SGD::Key::S) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsKeyPressed(SGD::Key::W) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;
	}
	else
	{
		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsKeyPressed(SGD::Key::S) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < (NUM_CHOICES + 1) ? m_nCursor + 1 : 0;
		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsKeyPressed(SGD::Key::W) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : (NUM_CHOICES + 1) - 1;
	}


	if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonPressed(0, 1) == true || pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
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
						Game::GetInstance()->ClearStateMachine();
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
	

	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	if (HTPGameState::GetInstance()->GetIsCurrState() == true)
	{
		HTPGameState::GetInstance()->Render();
		pGraphics->DrawRectangle({ 0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() }, { 210, 0, 0, 0 });
	}

	else if (HTPGameState::GetInstance()->GetIsCurrState() == false)
	{
		//HTPGameState::GetInstance()->SetIsCurrState(false);

		GameplayState::GetInstance()->Render();
		pGraphics->DrawRectangle({0,0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() }, { 210, 0, 0, 0 });

		//pGraphics->DrawRectangle({ 0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() }, { 210, 0, 0, 0 });
	}


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();
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

	// Draw the reticle
	SGD::Point	retpos = SGD::InputManager::GetInstance()->GetMousePosition();
	float		retscale = 0.8f;

	retpos.Offset(-32.0F * retscale, -32.0F * retscale);
	pGraphics->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });
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
