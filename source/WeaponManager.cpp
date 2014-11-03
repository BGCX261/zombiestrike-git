#include "WeaponManager.h"
#include "MovingObject.h"
#include "AssaultRifle.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "Sniper.h"
#include "FlameThrower.h"
#include "P90.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "GameplayState.h"
#include "Game.h"

#include "BitmapFont.h"

#include <cassert>
#include <sstream>

using namespace std;

/*static*/ WeaponManager* WeaponManager::GetInstance(void)
{
	static WeaponManager s_Instance;
	return &s_Instance;
}

void WeaponManager::Initialize(MovingObject& owner)
{
	m_hHudWpn = &GameplayState::GetInstance()->m_hHudWpn;
	m_hWpnSwitch = &GameplayState::GetInstance()->m_hWpnSwitch;

	SetOwner(&owner);
	
	AddWeapons(CreatePistol());
	AddWeapons(CreateShotgun());
	AddWeapons(CreateP90());
	AddWeapons(CreateAssaultRifle());
	AddWeapons(CreateSniper());
	
	//AddWeapons(CreateFlameThrower());

	curIndex = 0;
}

void WeaponManager::Render()
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	const BitmapFont * bFont = Game::GetInstance()->GetFont();

	SGD::Rectangle equipRect = { Game::GetInstance()->GetScreenWidth() - 275.0f, Game::GetInstance()->GetScreenHeight() - 150.0f, Game::GetInstance()->GetScreenWidth() - 10, Game::GetInstance()->GetScreenHeight() - 10 };

	pGraphics->DrawRectangle(equipRect, { 255, 255, 255 }, { 0, 0, 255 });

	for (unsigned int i = 0; i < m_vWeapons.size(); i++)
	{
		if (m_vWeapons[curIndex]->GetType() == m_vWeapons[i]->GetType() && m_vWeapons[curIndex]->GetObtained() == true)
		{
			pGraphics->DrawTextureSection(*m_hHudWpn, { Game::GetInstance()->GetScreenWidth() - 150.0f, Game::GetInstance()->GetScreenHeight() - 150.0f },
				m_vWeapons[curIndex]->GetRenderRect(), {}, {}, {}, { .5f, .5f });

			stringstream magSize;
			magSize << m_vWeapons[curIndex]->GetCurrAmmo() << "|";

			SGD::Point magPos = { equipRect.left + 10, equipRect.bottom - 50 };
			SGD::Point ammoPos = { equipRect.left + 71, equipRect.bottom - 50 };

			if (m_vWeapons[curIndex]->GetCurrAmmo() < 10)
			{
				magPos.x = equipRect.left + 28;
			}

			bFont->Draw(magSize.str().c_str(), magPos, 1.0f, { 0, 0, 0 });

			stringstream ammoCap;
			ammoCap << m_vWeapons[curIndex]->GetTotalAmmo();

			if (m_vWeapons[curIndex]->GetTotalAmmo() < 10)
			{
				ammoPos.x = equipRect.left + 58;
			}

			else if (m_vWeapons[curIndex]->GetTotalAmmo() < 100)
			{
				ammoPos.x = equipRect.left + 68;
			}

			else
			{
				ammoPos.x = equipRect.left + 71;
			}

			if (m_vWeapons[curIndex]->GetType() == PISTOL)
			{
				bFont->Draw("INF", ammoPos, 1.0f, { 0, 0, 0 });
			}

			else
				bFont->Draw(ammoCap.str().c_str(), ammoPos, 1.0f, { 0, 0, 0 });
			
		}
	}

	int size = 75;
	float sWidth = Game::GetInstance()->GetScreenWidth() / 2 - size * 2 - 50;
	float sHeight = Game::GetInstance()->GetScreenHeight() - 10;


	for (unsigned int i = 0; i < m_vWeapons.size(); i++)
	{		
		SGD::Rectangle unEquip = { sWidth + size*i, sHeight - 75, sWidth + size*i + size, sHeight };
		pGraphics->DrawRectangle(unEquip, { 255, 255, 255 }, { 0, 0, 255});

		if (m_vWeapons[i]->GetObtained() == true)
		{
			SGD::Rectangle imageRect = m_vWeapons[i]->GetRenderRect();

			if (m_vWeapons[i]->GetType() == m_vWeapons[curIndex]->GetType())
			{
				pGraphics->DrawTextureSection(*m_hHudWpn, { sWidth + size*i, sHeight - size },
					imageRect, {}, {}, {}, { .25f, .25f });
			}

			else
			{
				pGraphics->DrawTextureSection(*m_hHudWpn, { sWidth + size*i, sHeight - size },
					imageRect, {}, {}, {}, { .25f, .25f });
				pGraphics->DrawRectangle({ sWidth + size*i, sHeight - size, sWidth + size*i + size, sHeight }, { 175, 0, 0, 0 });
			}
		}

		stringstream drawIndex;
		drawIndex << i + 1;

		bFont->Draw(drawIndex.str().c_str(), { unEquip.left + 1, unEquip.top - 5 }, .5f, { 150, 155, 155 });

	}

	for (unsigned int i = 0; i < m_vWeapons.size(); i++)
	{
		SGD::Rectangle unEquip = { sWidth + size*i, sHeight - 75, sWidth + size*i + size, sHeight };

		if (m_vWeapons[i]->GetType() == m_vWeapons[curIndex]->GetType())
		{
			pGraphics->DrawRectangle({ sWidth + size*i, sHeight - size, sWidth + size*i + size, sHeight }, { 0, 0, 0, 0 }, { 0, 100, 0 },6);
		}

		stringstream drawIndex;
		drawIndex << i + 1;
		
		bFont->Draw(drawIndex.str().c_str(), { unEquip.left + 1, unEquip.top - 5}, .5f, { 150, 155, 155 });
	}

	

}

void WeaponManager::Input()
{
	SGD::InputManager * pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Q) == true)
	{
		curIndex--;

		if (curIndex < 0)
		{
			curIndex = m_vWeapons.size() - 1;
		}

		//for (unsigned int i = 0; i < 4; i++)
		//{
		//	drawIndex[i]--;
		//}

		if (pAudio->IsAudioPlaying(*m_hWpnSwitch) == false)
		{
			pAudio->PlayAudio(*m_hWpnSwitch, false);
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

		if (pAudio->IsAudioPlaying(*m_hWpnSwitch) == false)
		{
			pAudio->PlayAudio(*m_hWpnSwitch, false);
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

	if (pInput->IsKeyPressed(SGD::Key::R) == true && m_vWeapons[curIndex]->GetCurrAmmo() < m_vWeapons[curIndex]->GetMagSize())
	{
		//GetSelected()->SetCurrAmmo(0);
		//GetSelected()->GetReloadTimer().AddTime(GetSelected()->GetReloadTime());
		//m_vWeapons[curIndex]->SetCurrAmmo(0);
		//m_vWeapons[curIndex]->GetReloadTimer().AddTime(m_vWeapons[curIndex]->GetReloadTime());
	}
}

void WeaponManager::Update(float dt)
{
	Input();

	for (unsigned int i = 0; i < m_vWeapons.size(); i++)
	{
		m_vWeapons[i]->Update(dt);
	}
	
}

void WeaponManager::Exit()
{
	for (unsigned int i = 0; i < m_vWeapons.size(); i++)
	{
		delete m_vWeapons[i];
		m_vWeapons[i] = nullptr;

	}

	m_vWeapons.clear();

	m_pOwner->Release();
	m_pOwner = nullptr;
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
	AssaultRifle * ar = new AssaultRifle(GetOwner());

	//ar->SetOwner(GetOwner());
	ar->SetObtained(true);
	ar->SetRenderRect(SetImageRect(300, 300, 0, 1));
	ar->SetCurrAmmo(25);
	ar->SetTotalAmmo(150);
	

	return ar;
}

Weapon * WeaponManager::CreatePistol()
{
	Pistol * pistol = new Pistol(GetOwner());
	
	//pistol->SetOwner(GetOwner());
	pistol->SetObtained(true);
	pistol->SetRenderRect(SetImageRect(300, 300, 1, 3));
	pistol->SetCurrAmmo(10);
	pistol->SetTotalAmmo(INT_MAX);

	return pistol;
}

Weapon * WeaponManager::CreateShotgun()
{
	Shotgun * shotty = new Shotgun(GetOwner());

	//shotty->SetOwner(GetOwner());
	shotty->SetObtained(true);
	shotty->SetRenderRect(SetImageRect(300, 300, 0, 2));
	shotty->SetCurrAmmo(6);
	shotty->SetTotalAmmo(24);

	return shotty;
}

Weapon * WeaponManager::CreateSniper()
{
	Sniper * snip = new Sniper(GetOwner());

	//snip->SetOwner(GetOwner());
	snip->SetObtained(true);
	snip->SetRenderRect(SetImageRect(300, 300, 4, 0));
	snip->SetCurrAmmo(8);
	snip->SetTotalAmmo(16);

	return snip;
}

Weapon * WeaponManager::CreateFlameThrower()
{
	FlameThrower * ft = new FlameThrower(GetOwner());

	return ft;
}

Weapon * WeaponManager::CreateP90()
{
	P90 * p90 = new P90(GetOwner());

	//p90->SetOwner(GetOwner());
	p90->SetObtained(true);
	p90->SetRenderRect(SetImageRect(300, 300, 3, 0));
	p90->SetCurrAmmo(32);
	p90->SetTotalAmmo(96);

	return p90;
}


SGD::Rectangle WeaponManager::SetImageRect(float width, float height, unsigned int row, unsigned int col)
{
	float left = width * col;
	float top = height * row;

	return SGD::Rectangle( left, top, left + width, top + height );
}
