
#include "HUD.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

void HUD::Initialize(void)
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();

	// Load assets
	m_hBackgroundImage = pGraphics->LoadTexture("resource/graphics/HUD_clear3.png");
}
void HUD::Shutdown(void)
{
	SGD::GraphicsManager* pGraphics	= SGD::GraphicsManager::GetInstance();

	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);
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


	// Draw the HUD image
	float	screenheight	= Game::GetInstance()->GetScreenHeight();

	//pGraphics->DrawTextureSection(m_hBackgroundImage, { 1.5f, screenheight - 112.0f }, SGD::Rectangle(4.0f, 694.0f, 708.0f, 806.0f));
	pGraphics->DrawTexture(m_hBackgroundImage, { 1.5f, screenheight - 112.0f });
}