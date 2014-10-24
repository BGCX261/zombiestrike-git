#include "WeaponManager.h"
#include "AssaultRifle.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "Sniper.h"
#include "FlameThrower.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "GameplayState.h"
#include "Game.h"

#include <cassert>

vector<Weapon*> SelectWeapon(int index);

void WeaponManager::Initialize()
{
	m_hHudWpn = &GameplayState::GetInstance()->m_hHudWpn;

	AddWeapons(CreatePistol());
	AddWeapons(CreateShotgun());
	AddWeapons(CreateAssaultRifle());
	AddWeapons(CreateSniper());
	AddWeapons(CreateFlameThrower());

	curIndex = 2;
}

void WeaponManager::Render()
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	if (m_vWeapons[curIndex]->GetType() == Type::ASSUALT_RIFLE)
	{
		SGD::Rectangle arRect = SetImageRect(300, 300, 0, 1);

		pGraphics->DrawTextureSection(*m_hHudWpn, { Game::GetInstance()->GetScreenWidth() - 300.0f, Game::GetInstance()->GetScreenHeight() - 300.0f },
			arRect);
	}
}

void WeaponManager::Input()
{
	SGD::InputManager * pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Q) == true)
	{
		curIndex--;

		if (curIndex < 0)
		{
			curIndex = m_vWeapons.size() - 1;
		}

		while (m_vWeapons[curIndex]->GetObtained() != true)
		{
			curIndex--;		

			if (curIndex < 0)
			{
				curIndex = m_vWeapons.size() - 1;
			}
		}
	}

	if (pInput->IsKeyPressed(SGD::Key::E) == true)
	{
		curIndex++;

		if (curIndex > m_vWeapons.size() - 1)
		{
			curIndex = 0;
		}

		while (m_vWeapons[curIndex]->GetObtained() != true)
		{
			curIndex++;

			if (curIndex > m_vWeapons.size() - 1)
			{
				curIndex = 0;
			}
		}
	}
}

void WeaponManager::Update(float dt)
{
	Input();
}

void WeaponManager::Exit()
{

}

void WeaponManager::SelectWeapon(int index)
{
	if (index > 0 && index < m_vWeapons.size() - 1 && m_vWeapons[index]->GetObtained() == true)
	{
		curIndex = index;
	}
}

void WeaponManager::AddWeapons(Weapon* wpn)
{
	m_vWeapons.push_back(wpn);
}

Weapon * WeaponManager::CreateAssaultRifle()
{
	AssaultRifle * ar = new AssaultRifle();

	return ar;
}

Weapon * WeaponManager::CreatePistol()
{
	Pistol * pistol = new Pistol();

	pistol->SetObtained(true);

	return pistol;
}

Weapon * WeaponManager::CreateShotgun()
{
	Shotgun * shotty = new Shotgun();

	return shotty;
}

Weapon * WeaponManager::CreateSniper()
{
	Sniper * snip = new Sniper();

	return snip;
}

Weapon * WeaponManager::CreateFlameThrower()
{
	FlameThrower * ft = new FlameThrower();

	return ft;
}

SGD::Rectangle WeaponManager::SetImageRect(float width, float height, unsigned int row, unsigned int col)
{
	float left = width * col;
	float top = height * row;

	return SGD::Rectangle( left, top, left + width, top + height );
}
