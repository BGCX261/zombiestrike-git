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

	m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/MenuImages/youWin.png");
	m_hReticleImage = pGraphics->LoadTexture("resource/graphics/MenuImages/Reticle3.png", { 0, 0, 0 });

}


/**************************************************************/
// Exit
/*virtual*/ void WinGameState::Exit( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);
	pGraphics->UnloadTexture(m_hReticleImage);

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

	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();
	float scale = 1.25f;
	if (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0)
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
	}


	SGD::Point mousePos = pInput->GetMousePosition();

	if (pInput->GetMouseMovement() != SGD::Vector() || (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0))
	{
		if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(width *0.5f - (3 * 32 * scale), (height * 0.5F) + 100.0f), SGD::Size(128, 64))))
			m_nCursor = 0;
		else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(width *0.5f - (3 * 32 * scale), (height * 0.5F) + 200.0f), SGD::Size(128, 64))))
			m_nCursor = 1;

	}
		

		if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonPressed(0, 1) == true || pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
	{
		if (GameplayState::GetInstance()->GetGameMode() == true)
		{
			Game::GetInstance()->OverWriteProfile(Game::GetInstance()->GetStoryProfile());
			Game::GetInstance()->LoadStoryProfiles();

		}

		else
		{
			Game::GetInstance()->OverWriteProfile(Game::GetInstance()->GetSurvivalProfile());
			Game::GetInstance()->LoadSurvivalProfiles();

		}

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

	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();
	float scale = 1.25f;
	// Draw the background image
	pGraphics->DrawTexture(m_hBackgroundImage, {width * 0.1f , -60 });


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	

	pFont->Draw("Play Again?", { (width - (11 * 20* scale)) / 2, height * 0.5F }, scale, { 255, 255, 0 });
	pFont->Draw("YES", { (width - (3 * 32 * scale)) / 2, (height * 0.5F) + 100.0f }, scale, { 0, 255, 0 });
	pFont->Draw("NO", { (width - (2 * 32 * scale)) / 2, (height * 0.5F) + 200.0f }, scale, { 0, 255, 0 });


	if (m_nCursor == 0)
		pGraphics->DrawTexture(m_hReticleImage, { width * 0.5f - 125.0f, (height * 0.5F) + 100.0f }, 0.0F, {}, { 0, 255, 0 }, { .5f, .5f });

	else
		pGraphics->DrawTexture(m_hReticleImage, { width * 0.5f - 125.0f, (height * 0.5F) + 200.0f }, 0.0F, {}, { 0, 255, 0 }, { .5f, .5f });

	// Draw the reticle
	SGD::Point	retpos = SGD::InputManager::GetInstance()->GetMousePosition();
	float		retscale = 0.8f;

	retpos.Offset(-32.0F * retscale, -32.0F * retscale);
	pGraphics->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });
}
