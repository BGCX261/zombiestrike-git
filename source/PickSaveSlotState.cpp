#define NUM_CHOICES 4
#define MODE_CHOICES 4


#include "PickSaveSlotState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "OptionsState.h"
#include "HTPGameState.h"
#include "IntroState.h"
#include "GamerProfile.h"
#include "SpawnManager.h"


/*static*/ PickSaveSlotState* PickSaveSlotState::GetInstance(void)
{
	static PickSaveSlotState s_Instance;

	return &s_Instance;
}


void PickSaveSlotState::Enter(void)
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 50, 50, 50 });	// dark gray
	m_nCursor = 0;


	// Load assets
	if (GameplayState::GetInstance()->GetGameMode())
	{
		for (unsigned int i = 0; i < 3; i++)
		{


			profiles[i] = Game::GetInstance()->GetSpecStoryProfile(i);
		}
	}
	else
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			profiles[i] = Game::GetInstance()->GetSpecSurvialProfile(i);
		}
	}

	// Load volume levels
	OptionsState::GetInstance()->LoadVolumes();
	m_hReticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/MenuImages/Reticle3.png", { 0, 0, 0 });

}
void PickSaveSlotState::Exit(void)
{
	modeChosen = false;
	m_nCursor = 0;
	currState = 0;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hReticleImage);

}

bool PickSaveSlotState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsButtonPressed(0, 2) == true)
	{
		if (modeChosen)
			m_nCursor = MenuItems::EXIT_2;
		else
			m_nCursor = States::EXIT_1;

	}

	//return false;	// quit game
	SGD::Point mousePos = pInput->GetMousePosition();
	float starting_y = Game::GetInstance()->GetScreenHeight() * 0.35f;
	float offset = Game::GetInstance()->GetScreenHeight() * 0.1f;
	float width = Game::GetInstance()->GetScreenWidth();

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

	if (modeChosen == false)
	{
		if (pInput->GetMouseMovement() != SGD::Vector() || (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0))
		{
			if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * NEW_GAME)), SGD::Size(256, 64))))
				m_nCursor = 0;
			else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * LOAD_GAME)), SGD::Size(256, 64))))
				m_nCursor = 1;
			else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * DELETE_SAVES)), SGD::Size(256, 64))))
				m_nCursor = 2;
			else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * EXIT_1)), SGD::Size(256, 64))))
				m_nCursor = 3;
		}

		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsKeyPressed(SGD::Key::S) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < MODE_CHOICES ? m_nCursor + 1 : 0;

		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsKeyPressed(SGD::Key::W) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : MODE_CHOICES - 1;




		if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonPressed(0, 1) == true || pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
		{
			if (m_nCursor == NEW_GAME)
			{
				currState = NEW_GAME;
				modeChosen = true;
			}

			else if (m_nCursor == LOAD_GAME)
			{
				currState = LOAD_GAME;
				modeChosen = true;
			}
			else if (m_nCursor == DELETE_SAVES)
			{
				currState = DELETE_SAVES;
				modeChosen = true;
			}

			else if (m_nCursor == EXIT_1)
				Game::GetInstance()->RemoveState();

			m_nCursor = 0;

		}
	}
	else
	{
		float starting_y = Game::GetInstance()->GetScreenHeight() * 0.25f;
		float offset = Game::GetInstance()->GetScreenHeight() * 0.2f;

		if (pInput->GetMouseMovement() != SGD::Vector() || (pInput->GetLeftJoystick(0).x != 0 || pInput->GetLeftJoystick(0).y != 0))
		{
			if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * SAVE1)), SGD::Size(256, 128))))
				m_nCursor = 0;
			else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * SAVE2)), SGD::Size(256, 128))))
				m_nCursor = 1;
			else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * SAVE3)), SGD::Size(256, 128))))
				m_nCursor = 2;
			else if (mousePos.IsWithinRectangle(SGD::Rectangle(SGD::Point((width *0.4f), starting_y + (offset * EXIT_2)), SGD::Size(256, 128))))
				m_nCursor = 3;

		}

		if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsKeyPressed(SGD::Key::S) == true || pInput->IsDPadPressed(0, SGD::DPad::Down) == true)
			m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;

		else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsKeyPressed(SGD::Key::W) == true || pInput->IsDPadPressed(0, SGD::DPad::Up) == true)
			m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;


		if (pInput->IsKeyPressed(SGD::Key::Enter) == true || pInput->IsButtonPressed(0, 1) == true || pInput->IsKeyReleased(SGD::Key::MouseLeft) == true)
		{
			switch (m_nCursor)
			{
			case MenuItems::SAVE1:
			case MenuItems::SAVE2:
			case MenuItems::SAVE3:
			{
									 Game::GetInstance()->selectedProfile = m_nCursor;

									 if (currState == NEW_GAME || currState == DELETE_SAVES)
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

										 if (currState == DELETE_SAVES)
										 {
											 // Load assets
											 if (GameplayState::GetInstance()->GetGameMode())
											 {
												 for (unsigned int i = 0; i < 3; i++)
												 {
													 profiles[i] = Game::GetInstance()->GetSpecStoryProfile(i);
												 }

											 }
											 else
											 {
												 for (unsigned int i = 0; i < 3; i++)
												 {
													 profiles[i] = Game::GetInstance()->GetSpecSurvialProfile(i);
												 }
											 }
										 }


									 }
									 if (currState == NEW_GAME || currState == LOAD_GAME)
									 {
										 Game::GetInstance()->RemoveState();
										 Game::GetInstance()->RemoveState();
										 //			Game::GetInstance()->AddState(GameplayState::GetInstance());
										 if (GameplayState::GetInstance()->GetGameMode() == true)
										 {
											 if (Game::GetInstance()->GetStoryProfile().wavesComplete > 0)
											 {
												Game::GetInstance()->AddState(GameplayState::GetInstance());
											 }

											 else
												Game::GetInstance()->AddState(HTPGameState::GetInstance());
											
										 }
											 
										 else if (GameplayState::GetInstance()->GetGameMode() == false)
										 {									
											 if (Game::GetInstance()->GetSurvivalProfile().wavesComplete > 0)
											 {
												 Game::GetInstance()->AddState(GameplayState::GetInstance());
											 }
											 else
												 Game::GetInstance()->AddState(HTPGameState::GetInstance());
										 }
										 else
											 Game::GetInstance()->AddState(GameplayState::GetInstance());

										 return true;
									 }



									 return true;



			}
				break;


			case MenuItems::EXIT_2:
				modeChosen = false;

				return true;

				break;
			}

		}
	}


	return true;
}
void PickSaveSlotState::Update(float elapsedTime)
{

}
void PickSaveSlotState::Render(void)
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


	// Display the game title centered at 4x scale
	const wchar_t* title1 = L"Save Slots";	// 10
	pFont->Draw(title1, { (width - (10 * 40)) / 2, 100 }, 2.5f, { 255, 255, 255 });




	if (modeChosen == false)
	{
		// Display the menu options centered at 1x scale
		float starting_y = Game::GetInstance()->GetScreenHeight() * 0.35f;
		float offset = Game::GetInstance()->GetScreenHeight() * 0.1f;
		switch (m_nCursor)
		{
		case 0:
			pFont->Draw("New Game", { (width - (8 * 32)) / 2, starting_y + (offset * NEW_GAME) },			// 300
				1.75f, { 255, 255, 255, 255 });
			pFont->Draw("Load Game", { (width - (9 * 32)) / 2, starting_y + (offset * LOAD_GAME) },			// 350
				1.75f, { 255, 0, 0 });
			pFont->Draw("Delete Saves", { (width - (10 * 32)) / 2, starting_y + (offset * DELETE_SAVES) },			// 350
				1.75f, { 255, 0, 0 });
			pFont->Draw("Back To Menu", { (width - (12 * 32)) / 2, starting_y + (offset * EXIT_1) },			// 450
				1.75f, { 255, 0, 0 });
			break;
		case 1:
			pFont->Draw("New Game", { (width - (8 * 32)) / 2, starting_y + (offset * NEW_GAME) },			// 300
				1.75f, { 255, 0, 0 });
			pFont->Draw("Load Game", { (width - (9 * 32)) / 2, starting_y + (offset * LOAD_GAME) },			// 350
				1.75f, { 255, 255, 255, 255 });
			pFont->Draw("Delete Saves", { (width - (10 * 32)) / 2, starting_y + (offset * DELETE_SAVES) },			// 350
				1.75f, { 255, 0, 0 });
			pFont->Draw("Back To Menu", { (width - (12 * 32)) / 2, starting_y + (offset * EXIT_1) },			// 450
				1.75f, { 255, 0, 0 });
			break;
		case 2:
			pFont->Draw("New Game", { (width - (8 * 32)) / 2, starting_y + (offset * NEW_GAME) },			// 300
				1.75f, { 255, 0, 0 });
			pFont->Draw("Load Game", { (width - (9 * 32)) / 2, starting_y + (offset * LOAD_GAME) },			// 350
				1.75f, { 255, 0, 0 });
			pFont->Draw("Delete Saves", { (width - (10 * 32)) / 2, starting_y + (offset * DELETE_SAVES) },			// 350
				1.75f, { 255, 255, 255, 255 });
			pFont->Draw("Back To Menu", { (width - (12 * 32)) / 2, starting_y + (offset * EXIT_1) },			// 450
				1.75f, { 255, 0, 0 });
			break;
		case 3:
			pFont->Draw("New Game", { (width - (8 * 32)) / 2, starting_y + (offset * NEW_GAME) },			// 300
				1.75f, { 255, 0, 0 });
			pFont->Draw("Load Game", { (width - (9 * 32)) / 2, starting_y + (offset * LOAD_GAME) },			// 350
				1.75f, { 255, 0, 0 });
			pFont->Draw("Delete Saves", { (width - (10 * 32)) / 2, starting_y + (offset * DELETE_SAVES) },			// 350
				1.75f, { 255, 0, 0 });
			pFont->Draw("Back To Menu", { (width - (12 * 32)) / 2, starting_y + (offset * EXIT_1) },			// 450
				1.75f, { 255, 255, 255, 255 });
			break;
		}




		//// Display the cursor next to the option

		//const char* output = "=           =";
		//int length = 10;

		//if (m_nCursor == States::EXIT_1)
		//{
		//	output = "=             =";
		//	length = 12;
		//}

		//pFont->Draw(output, { (width - (length * 32)) / 2 - 32.0F, 300.0f + 50 * m_nCursor }, 1.0f, { 0, 255, 0 });
	}
	else
	{
		// Display the menu options centered at 1x scale
		float starting_y = Game::GetInstance()->GetScreenHeight() * 0.25f;
		float offset = Game::GetInstance()->GetScreenHeight() * 0.2f;

		stringstream save1String;
		stringstream save2String;
		stringstream save3String;

		save1String << "Save Slot 1\n"
			<< profiles[0].time.tm_mon
			<< '-'
			<< profiles[0].time.tm_mday
			<< '-'
			<< profiles[0].time.tm_year
			<< '\n'
			<< profiles[0].time.tm_hour
			<< ':'
			<< profiles[0].time.tm_min
			<< ':'
			<< profiles[0].time.tm_sec
			<< '\n'
			<< "Wavess Complete: " << profiles[0].wavesComplete;


		save2String << "Save Slot 2\n"
			<< profiles[1].time.tm_mon
			<< '-'
			<< profiles[1].time.tm_mday
			<< '-'
			<< profiles[1].time.tm_year
			<< '\n'
			<< profiles[1].time.tm_hour
			<< ':'
			<< profiles[1].time.tm_min
			<< ':'
			<< profiles[1].time.tm_sec
			<< '\n'
			<< "Waves Complete: " << profiles[1].wavesComplete;
		save3String << "Save Slot 3\n"
			<< profiles[2].time.tm_mon
			<< '-'
			<< profiles[2].time.tm_mday
			<< '-'
			<< profiles[2].time.tm_year
			<< '\n'
			<< profiles[2].time.tm_hour
			<< ':'
			<< profiles[2].time.tm_min
			<< ':'
			<< profiles[2].time.tm_sec
			<< '\n'
			<< "Wave Complete: " << profiles[2].wavesComplete;

		switch (m_nCursor)
		{
		case 0:
			pFont->Draw(save1String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE1) },			// 300
				1.25f, { 255, 255, 255, 255 });
			pFont->Draw(save2String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE2) },			// 300
				1.25f, { 255, 0, 0 });
			pFont->Draw(save3String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE3) },			// 350
				1.25f, { 255, 0, 0 });

			pFont->Draw("Back", { (width - (4 * 32)) / 2, starting_y + (offset * EXIT_2) },			// 450
				1.75f, { 255, 0, 0 });
			break;
		case 1:
			pFont->Draw(save1String.str().c_str(), { (width - (10 * 32)) / 2, starting_y + (offset * SAVE1) },			// 300
				1.25f, { 255, 0, 0 });
			pFont->Draw(save2String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE2) },			// 300
				1.25f, { 255, 255, 255, 255 });
			pFont->Draw(save3String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE3) },			// 350
				1.25f, { 255, 0, 0 });

			pFont->Draw("Back", { (width - (4 * 32)) / 2, starting_y + (offset * EXIT_2) },			// 450
				1.75f, { 255, 0, 0 });
			break;
		case 2:
			pFont->Draw(save1String.str().c_str(), { (width - (10 * 32)) / 2, starting_y + (offset * SAVE1) },			// 300
				1.25f, { 255, 0, 0 });
			pFont->Draw(save2String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE2) },			// 300
				1.25f, { 255, 0, 0 });
			pFont->Draw(save3String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE3) },			// 350
				1.25f, { 255, 255, 255, 255 });

			pFont->Draw("Back", { (width - (4 * 32)) / 2, starting_y + (offset * EXIT_2) },			// 450
				1.75f, { 255, 0, 0 });
			break;
		case 3:
			pFont->Draw(save1String.str().c_str(), { (width - (10 * 32)) / 2, starting_y + (offset * SAVE1) },			// 300
				1.25f, { 255, 0, 0 });
			pFont->Draw(save2String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE2) },			// 300
				1.25f, { 255, 0, 0 });
			pFont->Draw(save3String.str().c_str(), { (width - (11 * 32)) / 2, starting_y + (offset * SAVE3) },			// 350
				1.25f, { 255, 0, 0 });

			pFont->Draw("Back", { (width - (4 * 32)) / 2, starting_y + (offset * EXIT_2) },			// 450
				1.75f, { 255, 255, 255, 255 });
			break;

		}






	}

	// Draw the reticle
	SGD::Point	retpos = SGD::InputManager::GetInstance()->GetMousePosition();
	float		retscale = 0.8f;

	retpos.Offset(-32.0F * retscale, -32.0F * retscale);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });

}