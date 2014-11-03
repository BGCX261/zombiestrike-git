#define NUM_CHOICES		5

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "PickSaveSlotState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include "ShopState.h"


/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ MainMenuState* MainMenuState::GetInstance( void )
{
	static MainMenuState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
//	- reset the game & load resources
/*virtual*/ void MainMenuState::Enter( void )
{
	SGD::GraphicsManager*	pGraphics	= SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio		= SGD::AudioManager::GetInstance();
	SGD::InputManager*		pInput		= SGD::InputManager::GetInstance();


	// Reset the cursor to the top (comment out to keep the last cursor position)
	//m_nCursor = 0;
	clicked		= false;
	selected	= false;
	//pInput->CheckForNewControllers();


	// Load assets
	m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/Silent_Strike_menu.png");
	m_hTitleImage		= pGraphics->LoadTexture("resource/graphics/Silent_Strike.png");
	m_hReticleImage		= pGraphics->LoadTexture("resource/graphics/Reticle3.png", { 0, 0, 0 });
	m_hButton1			= pGraphics->LoadTexture("resource/graphics/rectangle1.png");
	m_hButton2			= pGraphics->LoadTexture("resource/graphics/rectangle2.png");

	m_hButtonSwitchSFX	= pAudio->LoadAudio("resource/audio/button_switch.wav");
	m_hMenuChangeSFX	= pAudio->LoadAudio("resource/audio/menu_change.wav");


	// Load volume levels
	OptionsState::GetInstance()->LoadVolumes();


	// setup selection rects
	selectonrects	= new SGD::Rectangle[NUM_CHOICES];
	stringlengths	= new int[NUM_CHOICES];

	int lengths[NUM_CHOICES] = { 9, 11, 7, 7, 4 };

	for (size_t i = 0; i < NUM_CHOICES; i++)
		stringlengths[i] = lengths[i];

	float x_offset = 16.0f;
	float y_offset = 15.0f;

	for (size_t i = 0; i < NUM_CHOICES; i++)
	{
		selectonrects[i].left	= starting_x;
		selectonrects[i].left	-= x_offset;

		selectonrects[i].top	= starting_y + (vertical_offset * i);
		selectonrects[i].top	-= y_offset;

		selectonrects[i].right	= selectonrects[i].left + 256.0f;
		//selectonrects[i].right	+= x_offset;

		selectonrects[i].bottom = selectonrects[i].top + 32.0f;
		selectonrects[i].bottom	+= y_offset * 2;
	}
}


/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void MainMenuState::Exit( void )
{
	SGD::GraphicsManager* pGraphics	= SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*	  pAudio	= SGD::AudioManager::GetInstance();


	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);
	pGraphics->UnloadTexture(m_hTitleImage);
	pGraphics->UnloadTexture(m_hReticleImage);
	pGraphics->UnloadTexture(m_hButton1);
	pGraphics->UnloadTexture(m_hButton2);

	pAudio->UnloadAudio(m_hButtonSwitchSFX);
	pAudio->UnloadAudio(m_hMenuChangeSFX);


	// deallocate dynamic arrays
	delete[] selectonrects;
	delete[] stringlengths;

	selectonrects = nullptr;
	stringlengths = nullptr;

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool MainMenuState::Input( void )
{
	SGD::InputManager*	pInput	= SGD::InputManager::GetInstance();
	SGD::AudioManager*	pAudio	= SGD::AudioManager::GetInstance();

	m_mPrevious = m_nCursor;


	// Press Escape to quit
	if( pInput->IsKeyPressed( SGD::Key::Escape ) == true/* || pInput->IsButtonDown(0, 2) == true */)
		m_nCursor = MenuItems::EXIT;


	// Input: keyboard & D-pad
	if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
	{
		m_mPrevious = m_nCursor;
		m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
	{
		m_mPrevious = m_nCursor;
		m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;
	}


	// Input: L1 - Left Joystick
	if (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0)
	{
		SGD::Point mpoint;
		mpoint.x = pInput->GetMousePosition().x + pInput->GetLeftJoystick(0).x;
		mpoint.y = pInput->GetMousePosition().y + pInput->GetLeftJoystick(0).y;

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


	// Input: Mouse
	SGD::Point mousepos = pInput->GetMousePosition();
	bool collided = false;
	for (size_t i = 0; i < NUM_CHOICES; i++)
	{
		if (mousepos.IsWithinRectangle(selectonrects[i]) == true)
		{
			m_mPrevious	= m_nCursor;
			m_nCursor	= i;
			collided	= true;
		}
	}
	selected = collided;

	clicked = pInput->IsKeyDown(SGD::Key::LButton) == true ? true : false;
	/*
	if (pInput->IsKeyDown(SGD::Key::LButton) == true)
		clicked = true;
	else
		clicked = false;
	*/

	if (m_mPrevious != m_nCursor)
	{
		if (pAudio->IsAudioPlaying(m_hButtonSwitchSFX) == false)
			pAudio->PlayAudio(m_hButtonSwitchSFX, false);
	}



	// Selection
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true || (pInput->IsKeyReleased(SGD::Key::LButton) == true && selected == true) || pInput->IsButtonDown(0, 1) == true)
	{
		if (pAudio->IsAudioPlaying(m_hMenuChangeSFX) == false)
			pAudio->PlayAudio(m_hMenuChangeSFX, false);

		switch (m_nCursor)
		{
		case MenuItems::PLAY_GAME:
			{
				Game::GetInstance()->AddState(GameplayState::GetInstance());
				return true;
			}
			break;


		case MenuItems::HOW_TO_PLAY:
			{
				Game::GetInstance()->AddState(HowToPlayState::GetInstance());
				return true;
			}
			break;


		case MenuItems::OPTIONS:
			{
				Game::GetInstance()->AddState(OptionsState::GetInstance());
				return true;
			}
			break;


		case MenuItems::CREDITS:
			{
				Game::GetInstance()->AddState(CreditsState::GetInstance());
				return true;
			}
			break;


		case MenuItems::EXIT:
			{
				return false;	// quit the game
			}
			break;
		}
	}
	/*
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::LButton) == true)
	{
		if (m_nCursor == MenuItems::PLAY_GAME)
		{
			Game::GetInstance()->RemoveState();
			Game::GetInstance()->AddState(GameplayState::GetInstance());
			return true;
		}
		else if (m_nCursor == MenuItems::PLAY_GAME)
		{
			Game::GetInstance()->AddState(HowToPlayState::GetInstance());
			return true;
		}
		else if (mousepos.IsWithinRectangle(selectonrects[MenuItems::OPTIONS]) == true)
		{
			Game::GetInstance()->AddState(OptionsState::GetInstance());
			return true;
		}
		else if (mousepos.IsWithinRectangle(selectonrects[MenuItems::CREDITS]) == true)
		{
			Game::GetInstance()->AddState(CreditsState::GetInstance());
			return true;
		}
		else if (mousepos.IsWithinRectangle(selectonrects[MenuItems::EXIT]) == true)
		{
			return false;	// quit the game
		}
	}
	*/





	// keep playing
	return true;
}


/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void MainMenuState::Update( float elapsedTime )
{
}


/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void MainMenuState::Render( void )
{
	SGD::GraphicsManager*	pGraphics	= SGD::GraphicsManager::GetInstance();
	SGD::InputManager*		pInput		= SGD::InputManager::GetInstance();


	// Draw the background image
	pGraphics->DrawTexture(m_hBackgroundImage, { 0, 0 });


	// Draw the buttons
	for (size_t i = 0; i < NUM_CHOICES; i++)
		pGraphics->DrawTexture(m_hButton2, selectonrects[i].GetTopLeft(), 0.0f, {}, {}, { 1.0f, 1.0f });


	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Draw the game title
	// Align text based on window width
	float width			= Game::GetInstance()->GetScreenWidth();
	float titlescale	= 2.0f;
	float xpos			= (width - (256.0f * titlescale)) * 0.5f;
	pGraphics->DrawTextureSection(m_hTitleImage, { xpos, 10 }, { 3.0f, 76.0f, 256.0f, 174.0f }, 0.0f, {}, { 255, 255, 0, 0 }, { titlescale, titlescale });



	// Display the menu options centered at 1x scale
	std::string menuitems[NUM_CHOICES] = { "Play Game", "How to Play", "Options", "Credits", "Exit" };

	float offset = 100.0F;
	for (size_t i = 0; i < NUM_CHOICES; i++)
	{
		SGD::Color clr_normal	= { 255, 255, 0, 0 };		// red
		SGD::Color clr_selected = { 255, 255, 255, 255 };	// white
		SGD::Color clr_held		= { 255, 0, 0, 0 };			// black


		// not selected or colliding
		if (m_nCursor != i)
			pFont->Draw(menuitems[i].c_str(), { starting_x, starting_y + (vertical_offset * i) }, text_scale, clr_normal);


		// selected, not clicked, colliding
		else if (m_nCursor == i && clicked == false && selected == true)
			pFont->Draw(menuitems[i].c_str(), { starting_x, starting_y + (vertical_offset * i) }, text_scale, clr_selected);


		// selected, clicked, colliding
		else if (m_nCursor == i && clicked == true && selected == true)
			pFont->Draw(menuitems[i].c_str(), { starting_x, starting_y + (vertical_offset * i) }, text_scale, clr_held);


		// selected, clicked, not colliding
		else if (m_nCursor == i && clicked == true && selected == false)
			pFont->Draw(menuitems[i].c_str(), { starting_x, starting_y + (vertical_offset * i) }, text_scale, clr_selected);


		// selected, not clicked, not colliding
		else if (m_nCursor == i && clicked == false && selected == false)
			pFont->Draw(menuitems[i].c_str(), { starting_x, starting_y + (vertical_offset * i) }, text_scale, clr_selected);
	}


	// Display the cursor next to the option
	pFont->Draw("=", { starting_x - 32.0F, starting_y + offset * m_nCursor - 8.0f }, 1.0f, { 0, 255, 0 });


	// Draw the reticle
	SGD::Point	retpos		= pInput->GetMousePosition();
	float		retscale	= 0.8f;

	retpos.Offset(-32.0F * retscale, -32.0F * retscale);
	pGraphics->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });
}
