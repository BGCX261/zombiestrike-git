#include "PickSaveSlotState.h"


#define NUM_CHOICES 4



#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "OptionsState.h"



/*static*/ PickSaveSlotState* PickSaveSlotState::GetInstance(void)
{
	static PickSaveSlotState s_Instance;

	return &s_Instance;
}


void PickSaveSlotState::Enter(void)
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 50, 50, 50 });	// dark gray


	// Load assets


	// Load volume levels
	OptionsState::GetInstance()->LoadVolumes();
}
void PickSaveSlotState::Exit(void)
{
	
}

bool PickSaveSlotState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		m_nCursor = MenuItems::EXIT;
	//return false;	// quit game



	if (pInput->IsKeyPressed(SGD::Key::Down) == true)
	{
		m_nCursor = m_nCursor + 1 < NUM_CHOICES ? m_nCursor + 1 : 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true)
	{
		m_nCursor = m_nCursor - 1 >= 0 ? m_nCursor - 1 : NUM_CHOICES - 1;
	}



	if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{
		switch (m_nCursor)
		{
		case MenuItems::SAVE1:
		case MenuItems::SAVE2:
		case MenuItems::SAVE3:
		{
			Game::GetInstance()->selectedProfile = m_nCursor;
			Game::GetInstance()->RemoveState();
			Game::GetInstance()->RemoveState();
			Game::GetInstance()->AddState(GameplayState::GetInstance());
			return true;
		}
			break;


		case MenuItems::EXIT:
			Game::GetInstance()->RemoveState();
			return true;

			break;
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
	pFont->Draw(title1, { (width - (10 * 32 * 2.0f)) / 2, 50 }, 2.0f, { 255, 255, 255 });


	// Display the menu options centered at 1x scale
	float starting_y = 300.0F;
	float offset = 50.0F;

	pFont->Draw("Save Slot1", { (width - (10 * 32)) / 2, starting_y + (offset * SAVE1) },			// 300
		1.0f, { 255, 0, 0 });
	pFont->Draw("Save Slot2", { (width - (10 * 32)) / 2, starting_y + (offset * SAVE2) },			// 350
		1.0f, { 255, 0, 0 });
	pFont->Draw("Save Slot3", { (width - (10 * 32)) / 2, starting_y + (offset * SAVE3) },			// 400
		1.0f, { 255, 0, 0 });
	pFont->Draw("Back To Menu", { (width - (12 * 32)) / 2, starting_y + (offset * EXIT) },			// 450
		1.0f, { 255, 0, 0 });
	


	// Display the cursor next to the option

	const char* output = "=           =";
	int length = 10;

	if (m_nCursor == MenuItems::EXIT)
	{
		output = "=             =";
		length = 12;
	}

	pFont->Draw(output, { (width - (length * 32)) / 2 - 32.0F, 300.0f + 50 * m_nCursor }, 1.0f, { 0, 255, 0 });

}