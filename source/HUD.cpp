#include "HUD.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Player.h"
#include "BitmapFont.h"
#include <sstream>
#include "AnimationManager.h"
#include "GameplayState.h"
#include "HTPGameState.h"

using std::stringstream;


void HUD::Initialize(Player* player)
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();

	m_pPlayer = player;

	// Load assets
	m_hTurret = pGraphics->LoadTexture("resource/graphics/0908-Robots2.png");
}
void HUD::Shutdown(void)
{
	SGD::GraphicsManager* pGraphics	= SGD::GraphicsManager::GetInstance();

	// Unload assets
	pGraphics->UnloadTexture(m_hTurret);
}


void HUD::Update(float dt)
{
	if (nofiticationBar.isActive == true)
	{
		if (nofiticationBar.textBar.bottom < nofiticationBar.maxHeight) //if its active, open it up
			nofiticationBar.textBar.bottom += nofiticationBar.maxHeight * dt;
		if (nofiticationBar.textBar.bottom > nofiticationBar.maxHeight) // cap it off
			nofiticationBar.textBar.bottom = nofiticationBar.maxHeight;
	}
	else
	{
		if (nofiticationBar.textBar.bottom > 0) //if its active, open it up
			nofiticationBar.textBar.bottom -= nofiticationBar.maxHeight * dt;
		if (nofiticationBar.textBar.bottom < 0) // cap it off
			nofiticationBar.textBar.bottom = 0;
	}
}
void HUD::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	const BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Draw the HUD image
	float	width	= Game::GetInstance()->GetScreenWidth();
	float	height	= Game::GetInstance()->GetScreenHeight();


	SGD::Point camerapos = Game::GetInstance()->GetCurrState() == GameplayState::GetInstance()
		? GameplayState::GetInstance()->GetCamera()->GetPosition()
		: HTPGameState::GetInstance()->GetCamera()->GetPosition();



	// draw health bars
	SGD::Rectangle currhealth = { width * 0.05f, height * 0.05f, m_pPlayer->GetCurrHealth() / m_pPlayer->GetMaxHealth() * 200 + width * 0.05f, height * 0.05f + 35 };
	SGD::Rectangle maxhealth = { width * 0.05f, height * 0.05f, width * 0.05f + 200, height * 0.05f + 35 };
	pGraphics->DrawRectangle(maxhealth, { 0, 0, 0 });


	SGD::Color healthcolor;

	// 76 - 100 -> Green
	if (m_pPlayer->GetCurrHealth() > m_pPlayer->GetMaxHealth()* 0.75F)
		healthcolor = { 0, 255, 0 };

	// 26 - 75 -> Yellow
	else if (m_pPlayer->GetCurrHealth() <= m_pPlayer->GetMaxHealth() * 0.75F && m_pPlayer->GetCurrHealth() > m_pPlayer->GetMaxHealth() * 0.25f)
		healthcolor = { 255, 255, 0 };

	// 0 - 25 -> Red
	else if (m_pPlayer->GetCurrHealth() <= m_pPlayer->GetMaxHealth() * 0.25F)
		healthcolor = { 255, 0, 0 };

	pGraphics->DrawRectangle(currhealth, healthcolor);


	// draw health as a string
	int hp = static_cast<int>(m_pPlayer->GetCurrHealth());

	stringstream health;
	health << "HP: " << hp;
	pFont->Draw(health.str().c_str(), { width * 0.05f, height * 0.05f }, 1.0f, { 255, 255, 255 });



	// # of turrets
	SGD::Point turretpos = { width * 0.3f, height * 0.9f - 20.0f };
	//turretpos.Offset(-camerapos.x, -camerapos.y);
	pGraphics->DrawTextureSection(m_hTurret, turretpos, SGD::Rectangle(123, 387, 160, 450), 0.0f, {}, {}, { 1.3f, 1.3f });

	int numturrets = m_pPlayer->m_nNumTurrets;
	stringstream turrets;
	turrets << "x " << numturrets;

	SGD::Point numberpos = { width * 0.35f, height * 0.95f - 20.0f };
	//numberpos.Offset(-camerapos.x, -camerapos.y);
	pFont->Draw(turrets.str().c_str(), numberpos, 1.3f, { 255, 0, 0 });


}