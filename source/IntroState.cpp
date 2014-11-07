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
/*static*/ IntroState* IntroState::GetInstance(void)
{
	static IntroState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void IntroState::Enter(void)
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black


	// Load assets
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager *		pAudio = SGD::AudioManager::GetInstance();

	m_hBackgroundImage = pGraphics->LoadTexture("resource/graphics/emergencybroadcast.png");

	m_hEmergency = pAudio->LoadAudio("resource/audio/zombieemergency.wav");

	IntroTimer.AddTime(60);
	ScreenTimer.AddTime(.1f);

	pAudio->PlayAudio(m_hEmergency, false);

	//m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/youLose.png");
}


/**************************************************************/
// Exit
/*virtual*/ void IntroState::Exit(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);

	pAudio->UnloadAudio(m_hEmergency);
}


/**************************************************************/
// Input
/*virtual*/ bool IntroState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{

		//COMMENT IN WHEN AUDIO ADDED
		pAudio->StopAudio(m_hEmergency);

		Game::GetInstance()->RemoveState();
		Game::GetInstance()->AddState(GameplayState::GetInstance());
		return true;
	}



	// keep playing
	return true;
}


/**************************************************************/
// Update
/*virtual*/ void IntroState::Update(float elapsedTime)
{
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	if (IntroTimer.GetTime() < 0.0f)
	{
		pAudio->StopAudio(m_hEmergency);

		Game::GetInstance()->RemoveState();
		Game::GetInstance()->AddState(GameplayState::GetInstance());
	}

	if (IntroTimer.GetTime() < 25.0f)
	{
		if (ScreenTimer.GetTime() <= 0.0f && IntroTimer.GetTime() > 12.0f)
		{
			transBack -= 5;
			transTextFirst += 5;
			ScreenTimer.AddTime(.1f);
		}

		else if (IntroTimer.GetTime() < 12.0f)
		{
			if (ScreenTimer.GetTime() < 0.0f)
			{
				transText += 5;
				transTextFirst -= 5;
				ScreenTimer.AddTime(.1f);
			}

			//ScreenTimer.Update(elapsedTime);
		}
		
		ScreenTimer.Update(elapsedTime);
	}

	if (transBack <= 0)
	{
		transBack = 0;
	}

	if (transText >= 255)
	{
		transText = 255;
	}

	if (transTextFirst >= 255)
	{
		transTextFirst = 255;
	}

	if (transTextFirst < 0)
	{
		transTextFirst = 0;
	}

	IntroTimer.Update(elapsedTime);
}


/**************************************************************/
// Render
/*virtual*/ void IntroState::Render(void)
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Color backColor(transBack, 255, 255, 255);
	// Draw the background image
	pGraphics->DrawTexture(m_hBackgroundImage, { -175, 100 }, 0.0f, {}, backColor);

	if (IntroTimer.GetTime() < 20.0f)
	{
		SGD::Color colorTextOne(transTextFirst, 100, 0, 0);

		pFont->Draw("2 Weeks Later", { Game::GetInstance()->GetScreenWidth() / 2 - 150, Game::GetInstance()->GetScreenHeight() / 2 - 25 }, 1.0f, colorTextOne);

		SGD::Color colorText(transText, 100, 0, 0);

		pFont->Draw("Trying to survive", { Game::GetInstance()->GetScreenWidth() / 2 - 200, Game::GetInstance()->GetScreenHeight() / 2 - 25}, 1.0f, colorText);
	}

	pFont->Draw("Press 'ESC' to continue", { Game::GetInstance()->GetScreenWidth() / 2 - 175, Game::GetInstance()->GetScreenHeight() - 25 }, .5f, { 100, 0, 0 });
}
