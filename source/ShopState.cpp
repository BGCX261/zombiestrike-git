#include "ShopState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"

#define NUM_CHOICES 4

/*static*/ ShopState* ShopState::GetInstance(void)
{
	static ShopState s_Instance;

	return &s_Instance;
}

/*virtual*/ void	ShopState::Enter(void)					/*override*/
{
	m_tShopTimer.AddTime(10);
	//m_tShopTimer.SetMaxTime(30);
}

/*virtual*/ void	ShopState::Exit(void)					/*override*/
{

}

/*virtual*/ bool	ShopState::Input(void)					/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();



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

					return true;
		}
			break;
		case 3: // main menu
		{
					return true;
		}
			break;
		}
	}


	return true;	// keep playing
}

/*virtual*/ void	ShopState::Update(float elapsedTime)	/*override*/
{
	if (GetShopTimer().GetTime() <= 0.0f)
	{
		m_bTimerSet = true;

		SGD::Event msg("UNPAUSE");
		msg.SendEventNow();
		Game::GetInstance()->RemoveState();
	}
	
	m_tShopTimer.Update(elapsedTime);
}

/*virtual*/ void	ShopState::Render(void)				/*override*/
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();
	const BitmapFont * pFont = Game::GetInstance()->GetFont();

	stringstream wave;
	wave << "NEXT WAVE IN";

	stringstream timer;
	timer << (int)GetShopTimer().GetTime();

	pFont->Draw(wave.str().c_str(), { Game::GetInstance()->GetScreenWidth() - 175, Game::GetInstance()->GetScreenHeight() - 150 }, .5f, { 155, 0, 0 });
	pFont->Draw(timer.str().c_str(), { Game::GetInstance()->GetScreenWidth() - 150, Game::GetInstance()->GetScreenHeight() - 100 }, 2.0f, { 155, 0, 0 });

}
