#define NUM_CHOICES		4
#define SAVE_FILE		"OptionsVolumes.txt"

#include "OptionsState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include <fstream>
#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"
#include <sstream>
#include "GameplayState.h"
#include "HTPGameState.h"

/**************************************************************/
// GetInstance
/*static*/ OptionsState* OptionsState::GetInstance( void )
{
	static OptionsState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void OptionsState::Enter( void )
{
	// Reset the cursor to the top (commented out to keep the last cursor position)
	m_nCursor = 0;

	
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {50, 50, 50} );	// black


	// Load assets
	SGD::AudioManager* pAudio	= SGD::AudioManager::GetInstance();

	m_hBackgroundSFX			= pAudio->LoadAudio(L"resource/audio/shotgun_fire.wav");
	m_hReticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/MenuImages/Reticle3.png", { 0, 0, 0 });


}

/**************************************************************/
// Exit
/*virtual*/ void OptionsState::Exit( void )
{
	SaveVolumes();

	// Unload assets
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	if (pAudio->IsAudioPlaying(m_hBackgroundSFX) == true)
		pAudio->StopAudio(m_hBackgroundSFX);

	pAudio->UnloadAudio(m_hBackgroundSFX);

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hReticleImage);

}


/**************************************************************/
// Input
/*virtual*/ bool OptionsState::Input( void )
{
	SGD::GraphicsManager*	pGraphics	= SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio		= SGD::AudioManager::GetInstance();
	SGD::InputManager*		pInput		= SGD::InputManager::GetInstance();


	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || (m_nCursor == (NUM_CHOICES - 1) && pInput->IsKeyPressed(SGD::Key::Enter) == true) ||
		pInput->IsButtonPressed(0, 2) == true || (m_nCursor == (NUM_CHOICES - 1) && pInput->IsButtonReleased(0, 1) == true))
		Game::GetInstance()->RemoveState();


			mousePos = pInput->GetMousePosition();

	if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsKeyPressed(SGD::Key::S) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
		m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsKeyPressed(SGD::Key::W) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
		m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;

		if ((pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0)
			&& (pInput->GetLeftJoystick(0).x <= 1.0f
			&& pInput->GetLeftJoystick(0).y <= 1.0f
			&& pInput->GetLeftJoystick(0).x >= -1.0f
			&& pInput->GetLeftJoystick(0).y >= -1.0f))
		{


			SGD::Vector	joystick = pInput->GetLeftJoystick(0);
			float		stickmin = 0.250f;
			float		mousevel = 1.0f;

			mousePos += joystick;
			//if (joystick.x > stickmin)
			//	mousePos.x += mousevel;
			//else if (joystick.x < stickmin * -1.0f)
			//	mousePos.x -= mousevel;

			//if (joystick.y > stickmin)
			//	mousePos.y += mousevel;
			//else if (joystick.y < stickmin * -1.0f)
			//	mousePos.y -= mousevel;

			//if (mousePos.x < 0.0F)
			//	mousePos.x = 0.0F;
			//if (mousePos.y < 0.0F)
			//	mousePos.y = 0.0F;
			//if (mousePos.x > Game::GetInstance()->GetScreenWidth())
			//	mousePos.x = Game::GetInstance()->GetScreenWidth();
			//if (mousePos.y > Game::GetInstance()->GetScreenHeight())
			//	mousePos.y = Game::GetInstance()->GetScreenHeight();

				pInput->SetMousePosition(mousePos);
		}

		mousePos = pInput->GetMousePosition();

	

	

	

	int volumes[2];
	volumes[0] = pAudio->GetMasterVolume(SGD::AudioGroup::Music);
	volumes[1] = pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects);

	float back_start = 25.0f;
	float left_start = 50.0F;				// 50
	float right_start = Game::GetInstance()->GetScreenWidth() - 224.0F;		// 800
	float starting_y = 200.0F;
	float offset = 50.0F;
	float scale = 1.25f;
	int volOffset = 10;	// int volMin = 0, volMax = 100;

	if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(left_start + 500.0F, 300.0F), SGD::Size(64, 64))))
	{
		if (pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
			volumes[0] -= volOffset;
		m_nCursor = 0;
	}
	else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(left_start + 650.0F, 300.0F), SGD::Size(64, 64))))
	{
		if (pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
			volumes[0] += volOffset;
		m_nCursor = 0;

	}
	
	if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(left_start + 500.0F, 450.0F), SGD::Size(64, 64))))
	{
		if (pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
			volumes[1] -= volOffset;
		m_nCursor = 1;

	}
	else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(left_start + 650.0F, 450.0F), SGD::Size(64, 64))))
	{
		if (pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
			volumes[1] += volOffset;
	
		m_nCursor = 1;

	}
	if ((mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(left_start + 425.0F, 600.0F), SGD::Size(64, 64))) || mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(left_start + 600.0F, 600.0F), SGD::Size(64, 64)))))
	{
		if (pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
		{
			m_bFullScreen = !m_bFullScreen;
			pGraphics->Resize(SGD::Size(Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight()), m_bFullScreen);
			pInput->SetMousePosition({ Game::GetInstance()->GetScreenWidth() * 0.5f, Game::GetInstance()->GetScreenHeight() * 0.5f });

		}
		
		m_nCursor = 2;
	}

	if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point(50.0f, Game::GetInstance()->GetScreenHeight() - 100.0f), SGD::Size(64, 30))))
	{
		if (pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
			Game::GetInstance()->RemoveState();

		m_nCursor = 3;
	}

	switch (m_nCursor)
	{
	case 0: // Music
		{
				if (pInput->IsKeyPressed(SGD::Key::Right) == true || pInput->IsDPadPressed(0, SGD::DPad::Right) == true )
				volumes[0] += volOffset;
			else if (pInput->IsKeyPressed(SGD::Key::Left) == true || pInput->IsDPadPressed(0, SGD::DPad::Left) == true)
				volumes[0] -= volOffset;
		}
		break;

	case 1: // Sfx
		{
			if (pInput->IsKeyPressed(SGD::Key::Right) == true || pInput->IsDPadPressed(0, SGD::DPad::Right) == true)
				volumes[1] += volOffset;
			else if (pInput->IsKeyPressed(SGD::Key::Left) == true || pInput->IsDPadPressed(0, SGD::DPad::Left) == true)
				volumes[1] -= volOffset;
		}
		break;

	case 2: // full screen
		{
				if (pInput->IsKeyPressed(SGD::Key::Right) == true || pInput->IsKeyPressed(SGD::Key::Left) == true ||
					pInput->IsDPadPressed(0, SGD::DPad::Right) == true || pInput->IsDPadPressed(0, SGD::DPad::Left) == true)
				{
					m_bFullScreen = !m_bFullScreen;
					pGraphics->Resize(SGD::Size(Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight()), m_bFullScreen);
				}
		}
		break;
	}


	pAudio->SetMasterVolume(SGD::AudioGroup::Music, volumes[0]);
	pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, volumes[1]);



	// keep playing
	return true;
}


/**************************************************************/
// Update
/*virtual*/ void OptionsState::Update( float elapsedTime )
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// music
	if (m_nCursor == 0)
	{
		if (pAudio->IsAudioPlaying(m_hBackgroundSFX) == true)
			pAudio->StopAudio(m_hBackgroundSFX);
	}

	// sfx
	else if (m_nCursor == 1)
	{
		if (pAudio->IsAudioPlaying(m_hBackgroundSFX) == false)
			pAudio->PlayAudio(m_hBackgroundSFX, true);
	}

	// full screen
	else if (m_nCursor == 2)
	{
		if (pAudio->IsAudioPlaying(m_hBackgroundSFX) == true)
			pAudio->StopAudio(m_hBackgroundSFX);
	}
}


/**************************************************************/
// Render
/*virtual*/ void OptionsState::Render( void )
{
	SGD::AudioManager*		pAudio		= SGD::AudioManager::GetInstance();

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(SGD::Point(50.0f, Game::GetInstance()->GetScreenHeight() - 100.0f), SGD::Size(64, 30)), {}, { 0, 255, 0 }, 2);

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


	// Display the title centered at 4x scale
	pFont->Draw("OPTIONS", { (width - (7 * 32 * 4.0f)) / 2, 50 }, 3.0f, { 255, 255, 255 });


	// Align text based on window width
	float left_start	= 50.0F;				// 50
	float right_start	= width - 224.0F;		// 800
	float starting_y	= 200.0F;
	float offset		= 50.0F;
	float scale			=1.25f;
	

	SGD::OStringStream volumes[2];
	volumes[0] << pAudio->GetMasterVolume(SGD::AudioGroup::Music);
	volumes[1] << pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects);


	pFont->Draw("Music", { left_start, 250.0F }, scale, { 255, 0, 0 });
	pFont->Draw("Volume", { left_start + 100.0F, 300.0F }, scale, { 255, 0, 0 });

	pFont->Draw("Sound Effects", { left_start, 400.0F }, scale, { 255, 0, 0 });
	pFont->Draw("Volume", { left_start + 100.0F, 450.0F }, scale, { 255, 0, 0 });
	pFont->Draw("Full Screen", { left_start + 100.0F, 550.0F }, scale, { 255, 0, 0 });

	pFont->Draw(" < ", { left_start + 500.0F, 300.0F }, 1.75f, { 255, 0, 0 });
	pFont->Draw(" > ", { left_start + 650.0F, 300.0F }, 1.75f, { 255, 0, 0 });

	pFont->Draw(" < ", { left_start + 500.0F, 450.0F }, 1.75f, { 255, 0, 0 });
	pFont->Draw(" > ", { left_start + 650.0F, 450.0F }, 1.75f, { 255, 0, 0 });

	pFont->Draw(" < ", { left_start + 425.0F, 600.0F }, 1.75f, { 255, 0, 0 });
	pFont->Draw(" > ", { left_start + 600.0F, 600.0F }, 1.75f, { 255, 0, 0 });

	std::stringstream testString;
	testString << mousePos.x << ", " << mousePos.y << "   " << SGD::InputManager::GetInstance()->GetLeftJoystick(0).x << ", " << SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;
	//pFont->Draw(testString.str().c_str(), { 20, 20 }, 1.25f, { 255, 255, 255 });



	switch (m_nCursor)
	{
	case 0:
		pFont->Draw(volumes[0].str().c_str(), { left_start + 600.0F, 300.0F }, scale, { 255, 255, 255 });
		pFont->Draw(volumes[1].str().c_str(), { left_start + 600.0F, 450.0F }, scale, { 255, 0, 0 });

		if (m_bFullScreen == false)
			pFont->Draw("ON", { (width - (2 * 32 * scale)) / 2, 600.0F }, scale, { 255, 0, 0 });
		else
			pFont->Draw("OFF", { (width - (3 * 32 * scale)) / 2, 600.0F }, scale, { 255, 0, 0 });


		pFont->Draw("Back", { left_start, 700.0F }, 1.0f, { 255, 0, 0 });

		break;
	
	case 1:
		pFont->Draw(volumes[0].str().c_str(), { left_start + 600.0F, 300.0F }, scale, { 255, 0, 0 });
		pFont->Draw(volumes[1].str().c_str(), { left_start + 600.0F, 450.0F }, scale, { 255, 255, 255 });

		if (m_bFullScreen == false)
			pFont->Draw("ON", { (width - (2 * 32 * scale)) / 2, 600.0F }, scale, { 255, 0, 0 });
		else
			pFont->Draw("OFF", { (width - (3 * 32 * scale)) / 2, 600.0F }, scale, { 255, 0, 0 });


		pFont->Draw("Back", { left_start, 700.0F }, 1.0f, { 255, 0, 0 });

		break;
	case 2:
		pFont->Draw(volumes[0].str().c_str(), { left_start + 600.0F, 300.0F }, scale, { 255, 0, 0 });
		pFont->Draw(volumes[1].str().c_str(), { left_start + 600.0F, 450.0F }, scale, { 255, 0, 0 });

		if (m_bFullScreen == false)
			pFont->Draw("ON", { (width - (2 * 32 * scale)) / 2, 600.0F }, scale, { 255, 255, 255 });
		else
			pFont->Draw("OFF", { (width - (3 * 32 * scale)) / 2, 600.0F }, scale, { 255, 255, 255 });


		pFont->Draw("Back", { left_start, 700.0F }, 1.0f, { 255, 0, 0 });

		break;
	case 3:
		pFont->Draw(volumes[0].str().c_str(), { left_start + 600.0F, 300.0F }, scale, { 255, 0, 0 });
		pFont->Draw(volumes[1].str().c_str(), { left_start + 600.0F, 450.0F }, scale, { 255, 0, 0 });

		if (m_bFullScreen == false)
			pFont->Draw("ON", { (width - (2 * 32 * scale)) / 2, 600.0F }, scale, { 255, 0, 0 });
		else
			pFont->Draw("OFF", { (width - (3 * 32 * scale)) / 2, 600.0F }, scale, { 255, 0, 0 });


		pFont->Draw("Back", { left_start, 700.0F }, 1.0f, { 255, 255, 255 });

		break;
	}
	

	//const char* output = "=                          =";
	//SGD::Point position;

	//// music & sfx
	//position.x = (left_start + 100.0F) - 50.0F;
	//position.y = 300.0F + (150.0f * m_nCursor);



	//// full screen
	//if (m_nCursor == 2)
	//{
	//	output		= m_bFullScreen == true ? "=  =" : "=    =";
	//	int length	= m_bFullScreen == true ? 4 : 5;

	//	position.x	= (width - (length * 32 * scale)) * 0.5f;
	//	position.y	= 600.0F;
	//}
	//else if (m_nCursor == 3)
	//{
	//	output		= "=     =";
	//	position.x	= left_start - 40.0F;
	//	position.y	= 700.0F;
	//}

	//pFont->Draw( output, position, 1.0f, {255, 0, 0} );

	// Draw the reticle
	SGD::Point	retpos = mousePos;
	float		retscale = 0.8f;

	retpos.Offset(-32.0F * retscale, -32.0F * retscale);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });
}


/**************************************************************/
// LoadVolumes
void OptionsState::LoadVolumes( void ) const
{
	// File Input
	std::ifstream load(SAVE_FILE, std::ios_base::in);
	if (load.is_open())
	{
		// Load volumes
		int musicvol, sfxvol;
		load >> musicvol >> sfxvol;
		load.ignore(INT_MAX, '\n');


		// Set volumes
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, musicvol);
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, sfxvol);


		// Close
		load.close();
	}
}


/**************************************************************/
// SaveVolumes
void OptionsState::SaveVolumes( void ) const
{
	std::ofstream save(SAVE_FILE, std::ios_base::out/* | std::ios_base::trunc*/);
	if (save.is_open())
	{
		// Store volumes
		int musicvol, sfxvol;
		musicvol	= SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
		sfxvol		= SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);


		// Save volumes
		save << musicvol << '\t' << sfxvol << '\n';


		// Close
		save.close();
	}
}


