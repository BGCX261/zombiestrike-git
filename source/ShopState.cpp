#include "ShopState.h"
#include "WeaponManager.h"
#include "Weapon.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MapManager.h"
#include "BarbedWire.h"
#include "SandBag.h"
#include "LandMine.h"




// Singleton Accessor
/*static*/ ShopState* ShopState::GetInstance(void)
{
	static ShopState s_Instance;	// stored in global memory once
	return &s_Instance;
}


/**********************************************************/
// IGameState Interface:
void	ShopState::Enter(void)
{
	m_tShopTimer.AddTime(500);

	profile = Game::GetInstance()->GetProfile();

	screenSize = SGD::Size( Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() );

	weaponsImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/weapons.png");
	buyButton = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/rectangle1.png");
	upgradeButton = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/rectangle2.png");
	m_hReticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/crosshair.png");

	barbedwire.isBought = true;
	sandBag.isBought = true;
	landMine.isBought = true;

	LoadShopStatus();
	float startY = screenSize.height * .4f;

	for (unsigned int currWeapon = 0; currWeapon < 8; currWeapon++)
	{
		Buttons[currWeapon] = SGD::Rectangle(SGD::Point( screenSize.width * 0.5f, (float)startY ), SGD::Size(BUTTON_WIDTH, BUTTON_HEIGHT));
		startY += BUTTON_HEIGHT + 5;
	}
	Buttons[8] = SGD::Rectangle({ screenSize.width* 0.7f, screenSize.height * 0.7f }, SGD::Size(BUTTON_WIDTH, BUTTON_HEIGHT));

	startY = screenSize.height * .15f;
	DefenseButtons[0] = SGD::Rectangle(SGD::Point(screenSize.width * 0.5f, (float)startY), SGD::Size(BUTTON_WIDTH, BUTTON_HEIGHT));
	DefenseButtons[1] = DefenseButtons[0];
	DefenseButtons[1].bottom += BUTTON_HEIGHT + 5;
	DefenseButtons[1].top += BUTTON_HEIGHT + 5;

	DefenseButtons[2] = DefenseButtons[0];
	DefenseButtons[2].bottom += screenSize.height * 0.2;
	DefenseButtons[2].top += screenSize.height * 0.2f;
	DefenseButtons[3] = DefenseButtons[2];
	DefenseButtons[3].bottom += BUTTON_HEIGHT + 5;
	DefenseButtons[3].top += BUTTON_HEIGHT + 5;
	DefenseButtons[4] = DefenseButtons[3];
	DefenseButtons[4].bottom += BUTTON_HEIGHT + 5;
	DefenseButtons[4].top += BUTTON_HEIGHT + 5;

	DefenseButtons[5] = DefenseButtons[2];
	DefenseButtons[5].bottom += screenSize.height * 0.2f;
	DefenseButtons[5].top += screenSize.height * 0.2f;
	DefenseButtons[6] = DefenseButtons[5];
	DefenseButtons[6].bottom += screenSize.height * 0.2f;
	DefenseButtons[6].top += screenSize.height * 0.2f;


	barbedWires = MapManager::GetInstance()->GetBaredWire(); 
	sandBags = MapManager::GetInstance()->GetSandBags();
	landMines = MapManager::GetInstance()->GetLandMines();



	profile.money = INT_MAX;
}
void	ShopState::Exit(void)
{

}

bool	ShopState::Input(void)
{
	
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Q) == true)
	{
		currPage--;
		if (currPage < 0)
			currPage = 5;
		
	}
	if (pInput->IsKeyPressed(SGD::Key::E) == true)
	{
		currPage++;
		if (currPage > 5)
			currPage = 0;

	}
	if (pInput->IsKeyPressed(SGD::Key::Tab) == true)
		currTab++;
		
	SGD::Point mousePos = pInput->GetMousePosition();

	switch (currPage)
	{
	case PISTOLS:
	{
					if (currTab > 1)
						currTab = 0;

					if (currTab == 0)
					{
						if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
						{
							for (unsigned int currButton = 0; currButton < 3; currButton++)
							{
								if (mousePos.IsWithinRectangle(Buttons[currButton]))
								{
									if (currButton == 0)
									{
										if (pistolUpgrade.magSize.isMaxed == false)
										{
											if (profile.money >= 500)
											{
												pistolUpgrade.magSize.upgradedSkill.stat += 5;
												pistolUpgrade.magSize.upgradedSkill.currTier++;

												profile.money -= 500;

												if (pistolUpgrade.magSize.upgradedSkill.currTier == pistolUpgrade.magSize.upgradedSkill.maxTier)
													pistolUpgrade.magSize.isMaxed = true;
											}
												
												
										}
										
									}

									else if (currButton == 1)
									{
										if (pistolUpgrade.reloadTime.isMaxed == false)
										{
											if (profile.money >= 500)
											{
												profile.money -= 500;

												pistolUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
												pistolUpgrade.reloadTime.upgradedSkill.currTier++;

												if (pistolUpgrade.reloadTime.upgradedSkill.currTier == pistolUpgrade.reloadTime.upgradedSkill.maxTier)
													pistolUpgrade.reloadTime.isMaxed = true;
											}
										}
									}
									else if (currButton == 2)
									{
										if (pistolUpgrade.recoilTime.isMaxed == false)
										{
											if (profile.money >= 500)
											{
												profile.money -= 500;

												pistolUpgrade.recoilTime.upgradedSkill.stat -= 0.11f;
												pistolUpgrade.recoilTime.upgradedSkill.currTier++;

												if (pistolUpgrade.recoilTime.upgradedSkill.currTier == pistolUpgrade.recoilTime.upgradedSkill.maxTier)
													pistolUpgrade.recoilTime.isMaxed = true;
											}
										}
									}
								}
							}
						}
					
						
					}
					else
					{
						if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
						{
							for (unsigned int currButton = 0; currButton < 8; currButton++)
							{
								if (mousePos.IsWithinRectangle(Buttons[currButton]))
								{
									switch (currButton)
									{
									case 0:
										if (revolverUpgrade.magSize.isMaxed == false)
										{
											if (profile.money >= 600)
											{
												profile.money -= 600;

												revolverUpgrade.magSize.upgradedSkill.stat += 5;
												revolverUpgrade.magSize.upgradedSkill.currTier++;

												if (revolverUpgrade.magSize.upgradedSkill.currTier == revolverUpgrade.magSize.upgradedSkill.maxTier)
													revolverUpgrade.magSize.isMaxed = true;
											}
										}

										break;
									case 1:
										if (revolverUpgrade.reloadTime.isMaxed == false)
										{
											if (profile.money >= 600)
											{
												profile.money -= 600;
												revolverUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
												revolverUpgrade.reloadTime.upgradedSkill.currTier++;

												if (revolverUpgrade.reloadTime.upgradedSkill.currTier == revolverUpgrade.reloadTime.upgradedSkill.maxTier)
													revolverUpgrade.reloadTime.isMaxed = true;
											}
										}
										break;
									case 2:
										if (revolverUpgrade.recoilTime.isMaxed == false)
										{
											if (profile.money >= 600)
											{
												profile.money -= 600;
												revolverUpgrade.recoilTime.upgradedSkill.stat -= 0.11f;
												revolverUpgrade.recoilTime.upgradedSkill.currTier++;

												if (revolverUpgrade.recoilTime.upgradedSkill.currTier == revolverUpgrade.recoilTime.upgradedSkill.maxTier)
													revolverUpgrade.recoilTime.isMaxed = true;
											}
										}
										break;
									case 3:
										if (revolverUpgrade.penPower.isMaxed == false)
										{
											if (profile.money >= 600)
											{
												profile.money -= 600;
												revolverUpgrade.penPower.upgradedSkill.stat++;
												revolverUpgrade.penPower.upgradedSkill.currTier++;

												if (revolverUpgrade.penPower.upgradedSkill.currTier == revolverUpgrade.penPower.upgradedSkill.maxTier)
													revolverUpgrade.penPower.isMaxed = true;
											}
										}
										break;
									case 4:
										if (revolverUpgrade.damage.isMaxed == false)
										{
											if (profile.money >= 600.0f)
											{
												profile.money -= 600.0f;
												revolverUpgrade.damage.upgradedSkill.stat -= 0.11f;
												revolverUpgrade.damage.upgradedSkill.currTier++;

												if (revolverUpgrade.damage.upgradedSkill.currTier == revolverUpgrade.damage.upgradedSkill.maxTier)
													revolverUpgrade.damage.isMaxed = true;
											}
										}
										break;
									case 5:
										if (revolverUpgrade.ammoCap.isMaxed == false)
										{
											if (profile.money >= 600)
											{
												profile.money -= 600.0f;
												revolverUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
												revolverUpgrade.ammoCap.upgradedSkill.currTier++;

												if (revolverUpgrade.ammoCap.upgradedSkill.currTier == revolverUpgrade.ammoCap.upgradedSkill.maxTier)
													revolverUpgrade.ammoCap.isMaxed = true;
											}
										}
										break;
									case 6:
										if (revolverUpgrade.totalAmmo.isMaxed == false)
										{
											if (profile.money >= 600.0f)
											{
												profile.money -= 600.0f;
												revolverUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
												revolverUpgrade.totalAmmo.upgradedSkill.currTier++;

												if (revolverUpgrade.totalAmmo.upgradedSkill.currTier == revolverUpgrade.totalAmmo.upgradedSkill.maxTier)
													revolverUpgrade.totalAmmo.isMaxed = true;
											}
										}
										break;

									}
								}
							}
							if (mousePos.IsWithinRectangle(Buttons[8]))
							{
								if (revolverUpgrade.isBought == false)
								{
									if (profile.money >= 2000)
									{
										profile.money -= 2000;
										revolverUpgrade.isBought = true;
									}
								}
								else
								{
									//equipt code
								}

							}
						}
					}
									
					

	}
		
		break;
	case SHOTGUNS:
		if (currTab > 2)
			currTab = 0;

		if (currTab == 0)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 6; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (sawnOffUpgrade.recoilTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									sawnOffUpgrade.recoilTime.upgradedSkill.stat += 5;
									sawnOffUpgrade.recoilTime.upgradedSkill.currTier++;

									if (sawnOffUpgrade.recoilTime.upgradedSkill.currTier == sawnOffUpgrade.recoilTime.upgradedSkill.maxTier)
										sawnOffUpgrade.recoilTime.isMaxed = true;
								}
							}

							break;
						case 1:
							if (sawnOffUpgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									sawnOffUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									sawnOffUpgrade.reloadTime.upgradedSkill.currTier++;

									if (sawnOffUpgrade.reloadTime.upgradedSkill.currTier == sawnOffUpgrade.reloadTime.upgradedSkill.maxTier)
										sawnOffUpgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (sawnOffUpgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									sawnOffUpgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									sawnOffUpgrade.bulletSpread.upgradedSkill.currTier++;

									if (sawnOffUpgrade.bulletSpread.upgradedSkill.currTier == sawnOffUpgrade.bulletSpread.upgradedSkill.maxTier)
										sawnOffUpgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 3:
							if (sawnOffUpgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									sawnOffUpgrade.damage.upgradedSkill.stat++;
									sawnOffUpgrade.damage.upgradedSkill.currTier++;

									if (sawnOffUpgrade.damage.upgradedSkill.currTier == sawnOffUpgrade.damage.upgradedSkill.maxTier)
										sawnOffUpgrade.damage.isMaxed = true;
								}
							}
							break;
						case 4:
							if (sawnOffUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									sawnOffUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									sawnOffUpgrade.ammoCap.upgradedSkill.currTier++;

									if (sawnOffUpgrade.ammoCap.upgradedSkill.currTier == sawnOffUpgrade.ammoCap.upgradedSkill.maxTier)
										sawnOffUpgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 5:
							if (sawnOffUpgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									sawnOffUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									sawnOffUpgrade.totalAmmo.upgradedSkill.currTier++;

									if (sawnOffUpgrade.totalAmmo.upgradedSkill.currTier == sawnOffUpgrade.totalAmmo.upgradedSkill.maxTier)
										sawnOffUpgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8])) 
				{
					if (sawnOffUpgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							sawnOffUpgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}
				
				}
					
			}
		}
		else if (currTab == 1)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 7; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (pumpShotgunUpgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									pumpShotgunUpgrade.magSize.upgradedSkill.stat += 5;
									pumpShotgunUpgrade.magSize.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.magSize.upgradedSkill.currTier == pumpShotgunUpgrade.magSize.upgradedSkill.maxTier)
										pumpShotgunUpgrade.magSize.isMaxed = true;
								}
							}

							break;
						
						case 1:
							if (pumpShotgunUpgrade.recoilTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									pumpShotgunUpgrade.recoilTime.upgradedSkill.stat -= 0.11f;
									pumpShotgunUpgrade.recoilTime.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.recoilTime.upgradedSkill.currTier == pumpShotgunUpgrade.recoilTime.upgradedSkill.maxTier)
										pumpShotgunUpgrade.recoilTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (pumpShotgunUpgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									pumpShotgunUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									pumpShotgunUpgrade.reloadTime.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.reloadTime.upgradedSkill.currTier == pumpShotgunUpgrade.reloadTime.upgradedSkill.maxTier)
										pumpShotgunUpgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 3:
							if (pumpShotgunUpgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									pumpShotgunUpgrade.bulletSpread.upgradedSkill.stat++;
									pumpShotgunUpgrade.bulletSpread.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.bulletSpread.upgradedSkill.currTier == pumpShotgunUpgrade.bulletSpread.upgradedSkill.maxTier)
										pumpShotgunUpgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 4:
							if (pumpShotgunUpgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									pumpShotgunUpgrade.damage.upgradedSkill.stat -= 0.11f;
									pumpShotgunUpgrade.damage.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.damage.upgradedSkill.currTier == pumpShotgunUpgrade.damage.upgradedSkill.maxTier)
										pumpShotgunUpgrade.damage.isMaxed = true;
								}
							}
							break;
						case 5:
							if (pumpShotgunUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									pumpShotgunUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									pumpShotgunUpgrade.ammoCap.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.ammoCap.upgradedSkill.currTier == pumpShotgunUpgrade.ammoCap.upgradedSkill.maxTier)
										pumpShotgunUpgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 6:
							if (pumpShotgunUpgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									pumpShotgunUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									pumpShotgunUpgrade.totalAmmo.upgradedSkill.currTier++;

									if (pumpShotgunUpgrade.totalAmmo.upgradedSkill.currTier == pumpShotgunUpgrade.totalAmmo.upgradedSkill.maxTier)
										pumpShotgunUpgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (pumpShotgunUpgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							pumpShotgunUpgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		else if (currTab == 2)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 8; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (autoShotgunUpgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									autoShotgunUpgrade.magSize.upgradedSkill.stat += 5;
									autoShotgunUpgrade.magSize.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.magSize.upgradedSkill.currTier == autoShotgunUpgrade.magSize.upgradedSkill.maxTier)
										autoShotgunUpgrade.magSize.isMaxed = true;
								}
							}

							break;

						case 1:
							if (autoShotgunUpgrade.recoilTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									autoShotgunUpgrade.recoilTime.upgradedSkill.stat -= 0.11f;
									autoShotgunUpgrade.recoilTime.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.recoilTime.upgradedSkill.currTier == autoShotgunUpgrade.recoilTime.upgradedSkill.maxTier)
										autoShotgunUpgrade.recoilTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (autoShotgunUpgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									autoShotgunUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									autoShotgunUpgrade.reloadTime.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.reloadTime.upgradedSkill.currTier == autoShotgunUpgrade.reloadTime.upgradedSkill.maxTier)
										autoShotgunUpgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 3:
							if (autoShotgunUpgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									autoShotgunUpgrade.bulletSpread.upgradedSkill.stat++;
									autoShotgunUpgrade.bulletSpread.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.bulletSpread.upgradedSkill.currTier == autoShotgunUpgrade.bulletSpread.upgradedSkill.maxTier)
										autoShotgunUpgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 4:
							if (autoShotgunUpgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									autoShotgunUpgrade.damage.upgradedSkill.stat -= 0.11f;
									autoShotgunUpgrade.damage.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.damage.upgradedSkill.currTier == autoShotgunUpgrade.damage.upgradedSkill.maxTier)
										autoShotgunUpgrade.damage.isMaxed = true;
								}
							}
							break;
						case 5:
							if (autoShotgunUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									autoShotgunUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									autoShotgunUpgrade.ammoCap.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.ammoCap.upgradedSkill.currTier == autoShotgunUpgrade.ammoCap.upgradedSkill.maxTier)
										autoShotgunUpgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 6:
							if (autoShotgunUpgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									autoShotgunUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									autoShotgunUpgrade.totalAmmo.upgradedSkill.currTier++;

									if (autoShotgunUpgrade.totalAmmo.upgradedSkill.currTier == autoShotgunUpgrade.totalAmmo.upgradedSkill.maxTier)
										autoShotgunUpgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (autoShotgunUpgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							autoShotgunUpgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		break;
	case SMGS:
		if (currTab > 2)
			currTab = 0;

		if (currTab == 0)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 6; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (uziUpgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									uziUpgrade.magSize.upgradedSkill.stat += 5;
									uziUpgrade.magSize.upgradedSkill.currTier++;

									if (uziUpgrade.magSize.upgradedSkill.currTier == uziUpgrade.magSize.upgradedSkill.maxTier)
										uziUpgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (uziUpgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									uziUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									uziUpgrade.reloadTime.upgradedSkill.currTier++;

									if (uziUpgrade.reloadTime.upgradedSkill.currTier == uziUpgrade.reloadTime.upgradedSkill.maxTier)
										uziUpgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (uziUpgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									uziUpgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									uziUpgrade.bulletSpread.upgradedSkill.currTier++;

									if (uziUpgrade.bulletSpread.upgradedSkill.currTier == uziUpgrade.bulletSpread.upgradedSkill.maxTier)
										uziUpgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 3:
							if (uziUpgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									uziUpgrade.damage.upgradedSkill.stat++;
									uziUpgrade.damage.upgradedSkill.currTier++;

									if (uziUpgrade.damage.upgradedSkill.currTier == uziUpgrade.damage.upgradedSkill.maxTier)
										uziUpgrade.damage.isMaxed = true;
								}
							}
							break;
						case 4:
							if (uziUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									uziUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									uziUpgrade.ammoCap.upgradedSkill.currTier++;

									if (uziUpgrade.ammoCap.upgradedSkill.currTier == uziUpgrade.ammoCap.upgradedSkill.maxTier)
										uziUpgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 5:
							if (uziUpgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									uziUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									uziUpgrade.totalAmmo.upgradedSkill.currTier++;

									if (uziUpgrade.totalAmmo.upgradedSkill.currTier == uziUpgrade.totalAmmo.upgradedSkill.maxTier)
										uziUpgrade.totalAmmo.isMaxed = true;
								}
							}
							break;

						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (uziUpgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							uziUpgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		else if (currTab == 1)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 6; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (tech9Upgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									tech9Upgrade.magSize.upgradedSkill.stat += 5;
									tech9Upgrade.magSize.upgradedSkill.currTier++;

									if (tech9Upgrade.magSize.upgradedSkill.currTier == tech9Upgrade.magSize.upgradedSkill.maxTier)
										tech9Upgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (tech9Upgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									tech9Upgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									tech9Upgrade.reloadTime.upgradedSkill.currTier++;

									if (tech9Upgrade.reloadTime.upgradedSkill.currTier == tech9Upgrade.reloadTime.upgradedSkill.maxTier)
										tech9Upgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (tech9Upgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									tech9Upgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									tech9Upgrade.bulletSpread.upgradedSkill.currTier++;

									if (tech9Upgrade.bulletSpread.upgradedSkill.currTier == tech9Upgrade.bulletSpread.upgradedSkill.maxTier)
										tech9Upgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 3:
							if (tech9Upgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									tech9Upgrade.damage.upgradedSkill.stat++;
									tech9Upgrade.damage.upgradedSkill.currTier++;

									if (tech9Upgrade.damage.upgradedSkill.currTier == tech9Upgrade.damage.upgradedSkill.maxTier)
										tech9Upgrade.damage.isMaxed = true;
								}
							}
							break;
						case 4:
							if (tech9Upgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									tech9Upgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									tech9Upgrade.ammoCap.upgradedSkill.currTier++;

									if (tech9Upgrade.ammoCap.upgradedSkill.currTier == tech9Upgrade.ammoCap.upgradedSkill.maxTier)
										tech9Upgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 5:
							if (tech9Upgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									tech9Upgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									tech9Upgrade.totalAmmo.upgradedSkill.currTier++;

									if (tech9Upgrade.totalAmmo.upgradedSkill.currTier == tech9Upgrade.totalAmmo.upgradedSkill.maxTier)
										tech9Upgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (tech9Upgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							tech9Upgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		else if (currTab == 2)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 6; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (p90Upgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									p90Upgrade.magSize.upgradedSkill.stat += 5;
									p90Upgrade.magSize.upgradedSkill.currTier++;

									if (p90Upgrade.magSize.upgradedSkill.currTier == p90Upgrade.magSize.upgradedSkill.maxTier)
										p90Upgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (p90Upgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									p90Upgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									p90Upgrade.reloadTime.upgradedSkill.currTier++;

									if (p90Upgrade.reloadTime.upgradedSkill.currTier == p90Upgrade.reloadTime.upgradedSkill.maxTier)
										p90Upgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (p90Upgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									p90Upgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									p90Upgrade.bulletSpread.upgradedSkill.currTier++;

									if (p90Upgrade.bulletSpread.upgradedSkill.currTier == p90Upgrade.bulletSpread.upgradedSkill.maxTier)
										p90Upgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 3:
							if (p90Upgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									p90Upgrade.damage.upgradedSkill.stat++;
									p90Upgrade.damage.upgradedSkill.currTier++;

									if (p90Upgrade.damage.upgradedSkill.currTier == p90Upgrade.damage.upgradedSkill.maxTier)
										p90Upgrade.damage.isMaxed = true;
								}
							}
							break;
						case 4:
							if (p90Upgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									p90Upgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									p90Upgrade.ammoCap.upgradedSkill.currTier++;

									if (p90Upgrade.ammoCap.upgradedSkill.currTier == p90Upgrade.ammoCap.upgradedSkill.maxTier)
										p90Upgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 5:
							if (p90Upgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									p90Upgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									p90Upgrade.totalAmmo.upgradedSkill.currTier++;

									if (p90Upgrade.totalAmmo.upgradedSkill.currTier == p90Upgrade.totalAmmo.upgradedSkill.maxTier)
										p90Upgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (p90Upgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							p90Upgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		break;
	case ASSAULT_RIFLES:
		if (currTab > 2)
			currTab = 0;

		if (currTab == 0)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 7; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (ak47Upgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									ak47Upgrade.magSize.upgradedSkill.stat += 5;
									ak47Upgrade.magSize.upgradedSkill.currTier++;

									if (ak47Upgrade.magSize.upgradedSkill.currTier == ak47Upgrade.magSize.upgradedSkill.maxTier)
										ak47Upgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (ak47Upgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									ak47Upgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									ak47Upgrade.reloadTime.upgradedSkill.currTier++;

									if (ak47Upgrade.reloadTime.upgradedSkill.currTier == ak47Upgrade.reloadTime.upgradedSkill.maxTier)
										ak47Upgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (ak47Upgrade.recoilTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									ak47Upgrade.recoilTime.upgradedSkill.stat -= 0.5f;
									ak47Upgrade.recoilTime.upgradedSkill.currTier++;

									if (ak47Upgrade.recoilTime.upgradedSkill.currTier == ak47Upgrade.recoilTime.upgradedSkill.maxTier)
										ak47Upgrade.recoilTime.isMaxed = true;
								}
							}
							break;
						case 3:
							if (ak47Upgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									ak47Upgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									ak47Upgrade.bulletSpread.upgradedSkill.currTier++;

									if (ak47Upgrade.bulletSpread.upgradedSkill.currTier == ak47Upgrade.bulletSpread.upgradedSkill.maxTier)
										ak47Upgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 4:
							if (ak47Upgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									ak47Upgrade.damage.upgradedSkill.stat++;
									ak47Upgrade.damage.upgradedSkill.currTier++;

									if (ak47Upgrade.damage.upgradedSkill.currTier == ak47Upgrade.damage.upgradedSkill.maxTier)
										ak47Upgrade.damage.isMaxed = true;
								}
							}
							break;
						case 5:
							if (ak47Upgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									ak47Upgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									ak47Upgrade.ammoCap.upgradedSkill.currTier++;

									if (ak47Upgrade.ammoCap.upgradedSkill.currTier == ak47Upgrade.ammoCap.upgradedSkill.maxTier)
										ak47Upgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 6:
							if (ak47Upgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									ak47Upgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									ak47Upgrade.totalAmmo.upgradedSkill.currTier++;
									
									if (ak47Upgrade.totalAmmo.upgradedSkill.currTier == ak47Upgrade.totalAmmo.upgradedSkill.maxTier)
										ak47Upgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (ak47Upgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							ak47Upgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		else if (currTab == 1)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 7; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (m16Upgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									m16Upgrade.magSize.upgradedSkill.stat += 5;
									m16Upgrade.magSize.upgradedSkill.currTier++;

									if (m16Upgrade.magSize.upgradedSkill.currTier == m16Upgrade.magSize.upgradedSkill.maxTier)
										m16Upgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (m16Upgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									m16Upgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									m16Upgrade.reloadTime.upgradedSkill.currTier++;

									if (m16Upgrade.reloadTime.upgradedSkill.currTier == m16Upgrade.reloadTime.upgradedSkill.maxTier)
										m16Upgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (m16Upgrade.recoilTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									m16Upgrade.recoilTime.upgradedSkill.stat -= 0.5f;
									m16Upgrade.recoilTime.upgradedSkill.currTier++;

									if (m16Upgrade.recoilTime.upgradedSkill.currTier == m16Upgrade.recoilTime.upgradedSkill.maxTier)
										m16Upgrade.recoilTime.isMaxed = true;
								}
							}
							break;
						case 3:
							if (m16Upgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									m16Upgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									m16Upgrade.bulletSpread.upgradedSkill.currTier++;

									if (m16Upgrade.bulletSpread.upgradedSkill.currTier == m16Upgrade.bulletSpread.upgradedSkill.maxTier)
										m16Upgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 4:
							if (m16Upgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									m16Upgrade.damage.upgradedSkill.stat++;
									m16Upgrade.damage.upgradedSkill.currTier++;

									if (m16Upgrade.damage.upgradedSkill.currTier == m16Upgrade.damage.upgradedSkill.maxTier)
										m16Upgrade.damage.isMaxed = true;
								}
							}
							break;
						case 5:
							if (m16Upgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									m16Upgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									m16Upgrade.ammoCap.upgradedSkill.currTier++;

									if (m16Upgrade.ammoCap.upgradedSkill.currTier == m16Upgrade.ammoCap.upgradedSkill.maxTier)
										m16Upgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 6:
							if (m16Upgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									m16Upgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									m16Upgrade.totalAmmo.upgradedSkill.currTier++;

									if (m16Upgrade.totalAmmo.upgradedSkill.currTier == m16Upgrade.totalAmmo.upgradedSkill.maxTier)
										m16Upgrade.totalAmmo.isMaxed = true;
								}
							}
							break;

						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (m16Upgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							m16Upgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
				
		else if (currTab == 2)
		{
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 7; currButton++)
				{
					if (mousePos.IsWithinRectangle(Buttons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (lmgUpgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									lmgUpgrade.magSize.upgradedSkill.stat += 5;
									lmgUpgrade.magSize.upgradedSkill.currTier++;

									if (lmgUpgrade.magSize.upgradedSkill.currTier == lmgUpgrade.magSize.upgradedSkill.maxTier)
										lmgUpgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (lmgUpgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									lmgUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									lmgUpgrade.reloadTime.upgradedSkill.currTier++;

									if (lmgUpgrade.reloadTime.upgradedSkill.currTier == lmgUpgrade.reloadTime.upgradedSkill.maxTier)
										lmgUpgrade.reloadTime.isMaxed = true;
								}
							}
							break;
						case 2:
							if (lmgUpgrade.recoilTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									lmgUpgrade.recoilTime.upgradedSkill.stat -= 0.5f;
									lmgUpgrade.recoilTime.upgradedSkill.currTier++;

									if (lmgUpgrade.recoilTime.upgradedSkill.currTier == lmgUpgrade.recoilTime.upgradedSkill.maxTier)
										lmgUpgrade.recoilTime.isMaxed = true;
								}
							}
							break;
						case 3:
							if (lmgUpgrade.bulletSpread.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									lmgUpgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
									lmgUpgrade.bulletSpread.upgradedSkill.currTier++;

									if (lmgUpgrade.bulletSpread.upgradedSkill.currTier == lmgUpgrade.bulletSpread.upgradedSkill.maxTier)
										lmgUpgrade.bulletSpread.isMaxed = true;
								}
							}
							break;
						case 4:
							if (lmgUpgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									lmgUpgrade.damage.upgradedSkill.stat++;
									lmgUpgrade.damage.upgradedSkill.currTier++;

									if (lmgUpgrade.damage.upgradedSkill.currTier == lmgUpgrade.damage.upgradedSkill.maxTier)
										lmgUpgrade.damage.isMaxed = true;
								}
							}
							break;
						case 5:
							if (lmgUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									lmgUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									lmgUpgrade.ammoCap.upgradedSkill.currTier++;

									if (lmgUpgrade.ammoCap.upgradedSkill.currTier == lmgUpgrade.ammoCap.upgradedSkill.maxTier)
										lmgUpgrade.ammoCap.isMaxed = true;
								}
							}
							break;
						case 6:
							if (lmgUpgrade.totalAmmo.isMaxed == false)
							{
								if (profile.money >= 100)
								{
									profile.money -= 100;
									lmgUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
									lmgUpgrade.totalAmmo.upgradedSkill.currTier++;

									if (lmgUpgrade.totalAmmo.upgradedSkill.currTier == lmgUpgrade.totalAmmo.upgradedSkill.maxTier)
										lmgUpgrade.totalAmmo.isMaxed = true;
								}
							}
							break;
						
						}
					}
				}
				if (mousePos.IsWithinRectangle(Buttons[8]))
				{
					if (lmgUpgrade.isBought == false)
					{
						if (profile.money >= 2000)
						{
							profile.money -= 2000;
							lmgUpgrade.isBought = true;
						}
					}
					else
					{
						//equipt code
					}

				}
			}
		}
		break;

	case HEAVY:
	
			if (currTab > 2)
				currTab = 0;

			if (currTab == 0)
			{
				if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
				{
					for (unsigned int currButton = 0; currButton < 8; currButton++)
					{
						if (mousePos.IsWithinRectangle(Buttons[currButton]))
						{
							switch (currButton)
							{
							case 0:
								if (sniperUpgrade.magSize.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;

										sniperUpgrade.magSize.upgradedSkill.stat += 5;
										sniperUpgrade.magSize.upgradedSkill.currTier++;

										if (sniperUpgrade.magSize.upgradedSkill.currTier == sniperUpgrade.magSize.upgradedSkill.maxTier)
											sniperUpgrade.magSize.isMaxed = true;
									}
								}

								break;
							case 1:
								if (sniperUpgrade.reloadTime.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										sniperUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
										sniperUpgrade.reloadTime.upgradedSkill.currTier++;

										if (sniperUpgrade.reloadTime.upgradedSkill.currTier == sniperUpgrade.reloadTime.upgradedSkill.maxTier)
											sniperUpgrade.reloadTime.isMaxed = true;
									}
								}
								break;
							case 2:
								if (sniperUpgrade.recoilTime.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										sniperUpgrade.recoilTime.upgradedSkill.stat -= 0.5f;
										sniperUpgrade.recoilTime.upgradedSkill.currTier++;

										if (sniperUpgrade.recoilTime.upgradedSkill.currTier == sniperUpgrade.recoilTime.upgradedSkill.maxTier)
											sniperUpgrade.recoilTime.isMaxed = true;
									}
								}
								break;
							case 3:
								if (sniperUpgrade.bulletSpread.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										sniperUpgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
										sniperUpgrade.bulletSpread.upgradedSkill.currTier++;

										if (sniperUpgrade.bulletSpread.upgradedSkill.currTier == sniperUpgrade.bulletSpread.upgradedSkill.maxTier)
											sniperUpgrade.bulletSpread.isMaxed = true;
									}
								}
								break;
							case 4:
								if (sniperUpgrade.damage.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										sniperUpgrade.damage.upgradedSkill.stat++;
										sniperUpgrade.damage.upgradedSkill.currTier++;

										if (sniperUpgrade.damage.upgradedSkill.currTier == sniperUpgrade.damage.upgradedSkill.maxTier)
											sniperUpgrade.damage.isMaxed = true;
									}
								}
								break;
							case 5:
								if (sniperUpgrade.penPower.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										sniperUpgrade.penPower.upgradedSkill.stat -= 0.11f;
										sniperUpgrade.penPower.upgradedSkill.currTier++;

										if (sniperUpgrade.penPower.upgradedSkill.currTier == sniperUpgrade.penPower.upgradedSkill.maxTier)
											sniperUpgrade.penPower.isMaxed = true;
									}
								}
								break;
							case 6:
								if (sniperUpgrade.ammoCap.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										sniperUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
										sniperUpgrade.ammoCap.upgradedSkill.currTier++;

										if (sniperUpgrade.ammoCap.upgradedSkill.currTier == sniperUpgrade.ammoCap.upgradedSkill.maxTier)
											sniperUpgrade.ammoCap.isMaxed = true;
									}
								}
								break;
							
							
							case 7:
								if (sniperUpgrade.totalAmmo.isMaxed == false)
								{
									if (profile.money >= 100)
									{
										profile.money -= 100;
										sniperUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
										sniperUpgrade.totalAmmo.upgradedSkill.currTier++;

										if (sniperUpgrade.totalAmmo.upgradedSkill.currTier == sniperUpgrade.totalAmmo.upgradedSkill.maxTier)
											sniperUpgrade.totalAmmo.isMaxed = true;
									}
								}
								break;
							
							}
						}
					}
					if (mousePos.IsWithinRectangle(Buttons[8]))
					{
						if (sniperUpgrade.isBought == false)
						{
							if (profile.money >= 2000)
							{
								profile.money -= 2000;
								sniperUpgrade.isBought = true;
							}
						}
						else
						{
							//equipt code
						}

					}
				}
			}
			else if (currTab == 1)
			{
				if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
				{
					for (unsigned int currButton = 0; currButton < 7; currButton++)
					{
						if (mousePos.IsWithinRectangle(Buttons[currButton]))
						{
							switch (currButton)
							{
							case 0:
								if (flameUpgrade.magSize.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;

										flameUpgrade.magSize.upgradedSkill.stat += 5;
										flameUpgrade.magSize.upgradedSkill.currTier++;

										if (flameUpgrade.magSize.upgradedSkill.currTier == flameUpgrade.magSize.upgradedSkill.maxTier)
											flameUpgrade.magSize.isMaxed = true;
									}
								}

								break;
							case 1:
								if (flameUpgrade.reloadTime.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										flameUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
										flameUpgrade.reloadTime.upgradedSkill.currTier++;

										if (flameUpgrade.reloadTime.upgradedSkill.currTier == flameUpgrade.reloadTime.upgradedSkill.maxTier)
											flameUpgrade.reloadTime.isMaxed = true;
									}
								}
								break;
							
							case 2:
								if (flameUpgrade.bulletSpread.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										flameUpgrade.bulletSpread.upgradedSkill.stat -= 0.11f;
										flameUpgrade.bulletSpread.upgradedSkill.currTier++;

										if (flameUpgrade.bulletSpread.upgradedSkill.currTier == flameUpgrade.bulletSpread.upgradedSkill.maxTier)
											flameUpgrade.bulletSpread.isMaxed = true;
									}
								}
								break;
							case 3:
								if (flameUpgrade.damage.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										flameUpgrade.damage.upgradedSkill.stat++;
										flameUpgrade.damage.upgradedSkill.currTier++;

										if (flameUpgrade.damage.upgradedSkill.currTier == flameUpgrade.damage.upgradedSkill.maxTier)
											flameUpgrade.damage.isMaxed = true;
									}
								}
								break;
							case 4:
								if (flameUpgrade.bulletVelocity.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										flameUpgrade.bulletVelocity.upgradedSkill.stat -= 0.11f;
										flameUpgrade.bulletVelocity.upgradedSkill.currTier++;

										if (flameUpgrade.bulletVelocity.upgradedSkill.currTier == flameUpgrade.bulletVelocity.upgradedSkill.maxTier)
											flameUpgrade.bulletVelocity.isMaxed = true;
									}
								}
								break;
							case 5:
								if (flameUpgrade.ammoCap.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										flameUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
										flameUpgrade.ammoCap.upgradedSkill.currTier++;

										if (flameUpgrade.ammoCap.upgradedSkill.currTier == flameUpgrade.ammoCap.upgradedSkill.maxTier)
											flameUpgrade.ammoCap.isMaxed = true;
									}
								}
								break;
							case 6:
								if (flameUpgrade.totalAmmo.isMaxed == false)
								{
									if (profile.money >= 100)
									{
										profile.money -= 100;
										flameUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
										flameUpgrade.totalAmmo.upgradedSkill.currTier++;

										if (flameUpgrade.totalAmmo.upgradedSkill.currTier == flameUpgrade.totalAmmo.upgradedSkill.maxTier)
											flameUpgrade.totalAmmo.isMaxed = true;
									}
								}
								break;
							
							}
						}
					}
					if (mousePos.IsWithinRectangle(Buttons[8]))
					{
						if (flameUpgrade.isBought == false)
						{
							if (profile.money >= 2000)
							{
								profile.money -= 2000;
								flameUpgrade.isBought = true;
							}
						}
						else
						{
							//equipt code
						}

					}
				}
			}
			else if (currTab == 2)
			{
				if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
				{
					for (unsigned int currButton = 0; currButton < 6; currButton++)
					{
						if (mousePos.IsWithinRectangle(Buttons[currButton]))
						{
							switch (currButton)
							{
							case 0:
								if (nadeLauncherUpgrade.magSize.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;

										nadeLauncherUpgrade.magSize.upgradedSkill.stat += 5;
										nadeLauncherUpgrade.magSize.upgradedSkill.currTier++;

										if (nadeLauncherUpgrade.magSize.upgradedSkill.currTier == nadeLauncherUpgrade.magSize.upgradedSkill.maxTier)
											nadeLauncherUpgrade.magSize.isMaxed = true;
									}
								}

								break;
							case 1:
								if (nadeLauncherUpgrade.reloadTime.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										nadeLauncherUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
										nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier++;

										if (nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier == nadeLauncherUpgrade.reloadTime.upgradedSkill.maxTier)
											nadeLauncherUpgrade.reloadTime.isMaxed = true;
									}
								}
								break;
	
							case 2:
								if (nadeLauncherUpgrade.damage.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										nadeLauncherUpgrade.damage.upgradedSkill.stat++;
										nadeLauncherUpgrade.damage.upgradedSkill.currTier++;

										if (nadeLauncherUpgrade.damage.upgradedSkill.currTier == nadeLauncherUpgrade.damage.upgradedSkill.maxTier)
											nadeLauncherUpgrade.damage.isMaxed = true;
									}
								}
								break;
							case 3:
								if (nadeLauncherUpgrade.bulletVelocity.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										nadeLauncherUpgrade.bulletVelocity.upgradedSkill.stat++;
										nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier++;

										if (nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier == nadeLauncherUpgrade.bulletVelocity.upgradedSkill.maxTier)
											nadeLauncherUpgrade.bulletVelocity.isMaxed = true;
									}
								}
								break;
							case 4:
								if (nadeLauncherUpgrade.ammoCap.isMaxed == false)
								{
									if (profile.money >= 600)
									{
										profile.money -= 600;
										nadeLauncherUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
										nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier++;

										if (nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier == nadeLauncherUpgrade.ammoCap.upgradedSkill.maxTier)
											nadeLauncherUpgrade.ammoCap.isMaxed = true;
									}
								}
								break;
							case 5:
								if (nadeLauncherUpgrade.totalAmmo.isMaxed == false)
								{
									if (profile.money >= 100)
									{
										profile.money -= 100;
										nadeLauncherUpgrade.totalAmmo.upgradedSkill.stat -= 0.11f;
										nadeLauncherUpgrade.totalAmmo.upgradedSkill.currTier++;

										if (nadeLauncherUpgrade.totalAmmo.upgradedSkill.currTier == nadeLauncherUpgrade.totalAmmo.upgradedSkill.maxTier)
											nadeLauncherUpgrade.totalAmmo.isMaxed = true;
									}
								}
								break;
							case 8:
								if (nadeLauncherUpgrade.isBought == false)
								{
									if (profile.money >= 2000)
									{
										profile.money -= 2000;
										nadeLauncherUpgrade.isBought = true;
									}
								}
								else
								{
									//equipt code
								}
								break;
							}
						}
					}
					if (mousePos.IsWithinRectangle(Buttons[8]))
					{
						if (nadeLauncherUpgrade.isBought == false)
						{
							if (profile.money >= 2000)
							{
								profile.money -= 2000;
								nadeLauncherUpgrade.isBought = true;
							}
						}
						else
						{
							//equipt code
						}

					}
				}
			}
		
		break;

		case DEFENSE:
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true)
			{
				for (unsigned int currButton = 0; currButton < 6; currButton++)
				{
					if (mousePos.IsWithinRectangle(DefenseButtons[currButton]))
					{
						switch (currButton)
						{
						case 0:
							if (nadeLauncherUpgrade.magSize.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;

									nadeLauncherUpgrade.magSize.upgradedSkill.stat += 5;
									nadeLauncherUpgrade.magSize.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.magSize.upgradedSkill.currTier == nadeLauncherUpgrade.magSize.upgradedSkill.maxTier)
										nadeLauncherUpgrade.magSize.isMaxed = true;
								}
							}

							break;
						case 1:
							if (nadeLauncherUpgrade.reloadTime.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									nadeLauncherUpgrade.reloadTime.upgradedSkill.stat -= 0.5f;
									nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier == nadeLauncherUpgrade.reloadTime.upgradedSkill.maxTier)
										nadeLauncherUpgrade.reloadTime.isMaxed = true;
								}
							}
							break;

						case 2:
							if (nadeLauncherUpgrade.damage.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									nadeLauncherUpgrade.damage.upgradedSkill.stat++;
									nadeLauncherUpgrade.damage.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.damage.upgradedSkill.currTier == nadeLauncherUpgrade.damage.upgradedSkill.maxTier)
										nadeLauncherUpgrade.damage.isMaxed = true;
								}
							}
							break;
						case 3:
							if (nadeLauncherUpgrade.bulletVelocity.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									nadeLauncherUpgrade.bulletVelocity.upgradedSkill.stat++;
									nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier == nadeLauncherUpgrade.bulletVelocity.upgradedSkill.maxTier)
										nadeLauncherUpgrade.bulletVelocity.isMaxed = true;
								}
							}
							break;
						case 4:
							if (nadeLauncherUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									nadeLauncherUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier == nadeLauncherUpgrade.ammoCap.upgradedSkill.maxTier)
										nadeLauncherUpgrade.ammoCap.isMaxed = true;
								}
							}
						case 5:
							if (nadeLauncherUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									nadeLauncherUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier == nadeLauncherUpgrade.ammoCap.upgradedSkill.maxTier)
										nadeLauncherUpgrade.ammoCap.isMaxed = true;
								}
							}
						case 6:
							if (nadeLauncherUpgrade.ammoCap.isMaxed == false)
							{
								if (profile.money >= 600)
								{
									profile.money -= 600;
									nadeLauncherUpgrade.ammoCap.upgradedSkill.stat -= 0.11f;
									nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier++;

									if (nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier == nadeLauncherUpgrade.ammoCap.upgradedSkill.maxTier)
										nadeLauncherUpgrade.ammoCap.isMaxed = true;
								}
							}
						}
					}
				}
			}
						
			break;

	}

	
	return true;
}

void	ShopState::Update(float elapsedTime)
{
	if (profile.barbWire.isBought)
	{
		barbedWireRepairCost = 0;
		barbWireCurrHealth = 0;
		barbWireMaxHealth = 0;
		for (unsigned i = 0; i < barbedWires.size(); i++)
		{
			barbWireCurrHealth += barbedWires[i]->GetCurrHP();
			barbWireMaxHealth += barbedWires[i]->GetMaxHP();
		}
		if (barbWireCurrHealth / barbWireMaxHealth != 1)
		{
			barbedWireRepairCost = (barbWireCurrHealth / barbWireMaxHealth) * 500;
		}
	}

	if (profile.sandBag.isBought)
	{
		sandBagCurrHealth = 0;
		sandBagMaxHealth = 0;
		sandBagRepairCost = 0;
		for (unsigned i = 0; i < sandBags.size(); i++)
		{
			sandBagCurrHealth += sandBags[i]->GetCurrHP();
			sandBagMaxHealth += sandBags[i]->GetMaxHP();
		}

		if (sandBagCurrHealth / sandBagMaxHealth != 1)
		{
			sandBagRepairCost = (sandBagCurrHealth / sandBagMaxHealth) * 500;
		}
	}

	if (profile.landMine.isBought)
	{
		numLandMines = 0;
		for (unsigned i = 0; i < landMines.size(); i++)
		{
			if (landMines[i]->IsActive())
			{
				numLandMines++;
			}
		}

		if ((numLandMines / landMines.size()) != 1)
		{
			landMineRepairCost = (numLandMines / landMines.size()) * 100;

		}
	}
	



	if (GetShopTimer().GetTime() <= 0.0f)
	{
		m_bTimerSet = true;

		SGD::Event msg("UNPAUSE");
		msg.SendEventNow();
		Game::GetInstance()->RemoveState();
	}
	
	m_tShopTimer.Update(elapsedTime);
}
void	ShopState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	WeaponManager* pWeaponManager = WeaponManager::GetInstance(); 



	stringstream wave;
	wave << "NEXT WAVE IN";

	stringstream timer;
	timer << (int)GetShopTimer().GetTime();

	pFont->Draw(wave.str().c_str(), { Game::GetInstance()->GetScreenWidth() - 175, Game::GetInstance()->GetScreenHeight() - 150 }, .5f, { 155, 0, 0 });
	pFont->Draw(timer.str().c_str(), { Game::GetInstance()->GetScreenWidth() - 150, Game::GetInstance()->GetScreenHeight() - 100 }, 2.0f, { 155, 0, 0 });



	switch (currPage)
	{
	case PISTOLS:
	{
					pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
					pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
					pFont->Draw("Glock", { shotTab1.left + 20, shotTab1.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw("Revolver", { shotTab2.left + 20, shotTab2.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					if (currTab == 0)
					{
						//pistol
						string pistolMagLevel = "LeveL ";
						pistolMagLevel += std::to_string(pistolUpgrade.magSize.upgradedSkill.currTier);
						pistolMagLevel += " ";

						string pistolReloadLevel = "LeveL ";
						pistolReloadLevel += std::to_string(pistolUpgrade.reloadTime.upgradedSkill.currTier);
						pistolReloadLevel += " ";

						string pistolRecoilLevel = "LeveL ";
						pistolRecoilLevel += std::to_string(pistolUpgrade.recoilTime.upgradedSkill.currTier);
						pistolRecoilLevel += " ";

						pFont->Draw("Pistol: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
						pFont->Draw(pistolMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5, { 255, 255, 0, 0 });
						pFont->Draw("Magezine Size: ", { screenSize.width *.3f, Buttons[0].top }, 0.5, { 255, 255, 0, 0 });
						pFont->Draw(pistolReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5, { 255, 255, 0, 0 });
						pFont->Draw("ReLoaD SpeeD: ", { screenSize.width *.3f, Buttons[1].top }, 0.5, { 255, 255, 0, 0 });
						pFont->Draw(pistolRecoilLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
						pFont->Draw("RaTe of Fire: ", { screenSize.width *.3f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
						pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(103.0f, 47.0f), SGD::Size(140.0f, 86.0f)));
						
						for (size_t i = 0; i < 3; i++)
							pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });
						
						if (pistolUpgrade.magSize.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (pistolUpgrade.reloadTime.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });
						
						if (pistolUpgrade.recoilTime.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						

					}
					else
					{
						//Revolver
						string revolverMagLevel = "LeveL ";
						revolverMagLevel += std::to_string(revolverUpgrade.magSize.upgradedSkill.currTier);
						revolverMagLevel += " ";

						string revolverReloadLevel = "LeveL ";
						revolverReloadLevel += std::to_string(revolverUpgrade.reloadTime.upgradedSkill.currTier);
						revolverReloadLevel += " ";

						string revolverRecoilLevel = "LeveL ";
						revolverRecoilLevel += std::to_string(revolverUpgrade.recoilTime.upgradedSkill.currTier);
						revolverRecoilLevel += " ";

						string revolverAmmoCapLevel = "LeveL ";
						revolverAmmoCapLevel += std::to_string(revolverUpgrade.ammoCap.upgradedSkill.currTier);
						revolverAmmoCapLevel += " ";

						string revolverPenPowerLevel = "LeveL ";
						revolverPenPowerLevel += std::to_string(revolverUpgrade.penPower.upgradedSkill.currTier);
						revolverPenPowerLevel += " ";

						string revolverDamageLevel = "LeveL ";
						revolverDamageLevel += std::to_string(revolverUpgrade.damage.upgradedSkill.currTier);
						revolverDamageLevel += " ";

						pFont->Draw("Revolver: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
						pFont->Draw(revolverMagLevel.c_str(), { screenSize.width *0.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw("Magezine Size: ", { screenSize.width *0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw(revolverReloadLevel.c_str(), { screenSize.width *0.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw("ReLoaD SpeeD: ", { screenSize.width *0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw(revolverRecoilLevel.c_str(), { screenSize.width *0.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
						pFont->Draw("RaTe of Fire: ", { screenSize.width *0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw(revolverPenPowerLevel.c_str(), { screenSize.width *0.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw("Penetrating Power: ", { screenSize.width *0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw(revolverDamageLevel.c_str(), { screenSize.width *0.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw("Damage: ", { screenSize.width *0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw(revolverAmmoCapLevel.c_str(), { screenSize.width *0.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw("Ammo Capacity: ", { screenSize.width *0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
						pFont->Draw("Ammo: ", { screenSize.width *0.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
						pGraphics->DrawTextureSection(weaponsImage,  { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(315.0f, 560.0f), SGD::Size(180.0f, 95.0f)));

						for (size_t i = 0; i < 7; i++)
							pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });
						pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });

					
						if (revolverUpgrade.magSize.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.reloadTime.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.recoilTime.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.penPower.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.damage.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.ammoCap.isMaxed == false)
							pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.totalAmmo.isMaxed == false)
							pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.isBought == false)
							pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					}
					

				


	}

		break;
	case SHOTGUNS:
	{
					 pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
					 pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
					 pGraphics->DrawTexture(upgradeButton, { shotTab3.left, shotTab3.top }, {}, {}, {}, { 0.5f, 0.5f });
					 pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });

					 pFont->Draw("Sawn Off", { shotTab1.left + 20, shotTab1.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 pFont->Draw("P. AcTion", { shotTab2.left + 20, shotTab2.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 pFont->Draw("AuTo", { shotTab3.left + 45, shotTab3.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 //Revolver
					 switch (currTab)
					 {
					 case 0:
					 {
							   string sawnOffRecoilLevel = "LeveL ";
							   sawnOffRecoilLevel += std::to_string(sawnOffUpgrade.recoilTime.upgradedSkill.currTier);
							   sawnOffRecoilLevel += " ";

							   string sawnOffReloadLevel = "LeveL ";
							   sawnOffReloadLevel += std::to_string(sawnOffUpgrade.reloadTime.upgradedSkill.currTier);
							   sawnOffReloadLevel += " ";

							   string sawnOffBullSpreadLevel = "LeveL ";
							   sawnOffBullSpreadLevel += std::to_string(sawnOffUpgrade.bulletSpread.upgradedSkill.currTier);
							   sawnOffBullSpreadLevel += " ";

							   string sawnOffAmmoCapLevel = "LeveL ";
							   sawnOffAmmoCapLevel += std::to_string(sawnOffUpgrade.ammoCap.upgradedSkill.currTier);
							   sawnOffAmmoCapLevel += " ";

							   string sawnOffDamageLevel = "LeveL ";
							   sawnOffDamageLevel += std::to_string(sawnOffUpgrade.damage.upgradedSkill.currTier);
							   sawnOffDamageLevel += " ";

							   pFont->Draw("Sawn-Off: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
							   pFont->Draw(sawnOffRecoilLevel.c_str(), { screenSize.width *0.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Rate of Fire: ", { screenSize.width *0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(sawnOffReloadLevel.c_str(), { screenSize.width *0.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Reload Speed: ", { screenSize.width *0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(sawnOffBullSpreadLevel.c_str(), { screenSize.width *0.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
							   pFont->Draw("Shot Spread: ", { screenSize.width *0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(sawnOffDamageLevel.c_str(), { screenSize.width *0.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Damage: ", { screenSize.width *0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(sawnOffAmmoCapLevel.c_str(), { screenSize.width *0.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Ammo Capacity: ", { screenSize.width *0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Ammo: ", { screenSize.width *0.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							   pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(515.0f, 575.0f), SGD::Size(240.0f, 75.0f)));
							   for (size_t i = 0; i < 6; i++)
								   pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });
					
							  

							   if (sawnOffUpgrade.recoilTime.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.reloadTime.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });
							  
							   if (sawnOffUpgrade.bulletSpread.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });
							 
							   if (sawnOffUpgrade.damage.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.ammoCap.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.totalAmmo.isMaxed == false)
								   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.isBought == false)
								   pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 
					 }
						 break;
					 case 1:
					 {
							   string pumpMagSizeLevel = "LeveL ";
							   pumpMagSizeLevel += std::to_string(pumpShotgunUpgrade.magSize.upgradedSkill.currTier);
							   pumpMagSizeLevel += " ";

							   string pumpRecoilLevel = "LeveL ";
							   pumpRecoilLevel += std::to_string(pumpShotgunUpgrade.recoilTime.upgradedSkill.currTier);
							   pumpRecoilLevel += " ";

							   string pumpReloadLevel = "LeveL ";
							   pumpReloadLevel += std::to_string(pumpShotgunUpgrade.reloadTime.upgradedSkill.currTier);
							   pumpReloadLevel += " ";

							   string pumpBullSpreadLevel = "LeveL ";
							   pumpBullSpreadLevel += std::to_string(pumpShotgunUpgrade.bulletSpread.upgradedSkill.currTier);
							   pumpBullSpreadLevel += " ";

							   string pumpAmmoCapLevel = "LeveL ";
							   pumpAmmoCapLevel += std::to_string(pumpShotgunUpgrade.ammoCap.upgradedSkill.currTier);
							   pumpAmmoCapLevel += " ";

							   string pumpDamageLevel = "LeveL ";
							   pumpDamageLevel += std::to_string(pumpShotgunUpgrade.damage.upgradedSkill.currTier);
							   pumpDamageLevel += " ";

							   pFont->Draw("Pump Action Shotgun: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
							   pFont->Draw(pumpMagSizeLevel.c_str(), { screenSize.width *0.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Magezine Size: ", { screenSize.width *0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(pumpRecoilLevel.c_str(), { screenSize.width *0.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Rate of Fire: ", { screenSize.width *0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(pumpReloadLevel.c_str(), { screenSize.width *0.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Reload Speed: ", { screenSize.width *0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(pumpBullSpreadLevel.c_str(), { screenSize.width *0.1f, Buttons[3].top }, 0.5, { 255, 255, 0, 0 });
							   pFont->Draw("Shot Spread: ", { screenSize.width *0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(pumpDamageLevel.c_str(), { screenSize.width *0.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Damage: ", { screenSize.width *0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(pumpAmmoCapLevel.c_str(), { screenSize.width *0.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Ammo Capacity: ", { screenSize.width *0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Ammo: ", { screenSize.width *0.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
							   pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(15.0f, 225.0f), SGD::Size(218.0f, 70.0f)));
							   for (size_t i = 0; i < 7; i++)
								   pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });
							  

							   if (pumpShotgunUpgrade.magSize.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });
 
							   if (pumpShotgunUpgrade.recoilTime.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.reloadTime.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.bulletSpread.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.damage.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.ammoCap.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.totalAmmo.isMaxed == false)
								   pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.isBought == false)
								   pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });

					 
					 }
						 break;
					 case 2:
					 {
							   string autoMagSizeLevel = "LeveL ";
							   autoMagSizeLevel += std::to_string(autoShotgunUpgrade.magSize.upgradedSkill.currTier);
							   autoMagSizeLevel += " ";

							   string autoRecoilLevel = "LeveL ";
							   autoRecoilLevel += std::to_string(autoShotgunUpgrade.recoilTime.upgradedSkill.currTier);
							   autoRecoilLevel += " ";

							   string autoReloadLevel = "LeveL ";
							   autoReloadLevel += std::to_string(autoShotgunUpgrade.reloadTime.upgradedSkill.currTier);
							   autoReloadLevel += " ";

							   string autoBullSpreadLevel = "LeveL ";
							   autoBullSpreadLevel += std::to_string(autoShotgunUpgrade.bulletSpread.upgradedSkill.currTier);
							   autoBullSpreadLevel += " ";

							   string autoAmmoCapLevel = "LeveL ";
							   autoAmmoCapLevel += std::to_string(autoShotgunUpgrade.ammoCap.upgradedSkill.currTier);
							   autoAmmoCapLevel += " ";

							   string autoDamageLevel = "LeveL ";
							   autoDamageLevel += std::to_string(autoShotgunUpgrade.damage.upgradedSkill.currTier);
							   autoDamageLevel += " ";

							   pFont->Draw("Auto Shotgun: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
							   pFont->Draw(autoMagSizeLevel.c_str(), { screenSize.width *0.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Magezine Size: ", { screenSize.width *0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(autoRecoilLevel.c_str(), { screenSize.width *0.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Rate of Fire: ", { screenSize.width *0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(autoReloadLevel.c_str(), { screenSize.width *0.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Reload Speed: ", { screenSize.width *0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(autoBullSpreadLevel.c_str(), { screenSize.width *0.1f, Buttons[3].top }, 0.5, { 255, 255, 0, 0 });
							   pFont->Draw("Shot Spread: ", { screenSize.width *0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(autoDamageLevel.c_str(), { screenSize.width *0.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Damage: ", { screenSize.width *0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw(autoAmmoCapLevel.c_str(), { screenSize.width *0.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Ammo Capacity: ", { screenSize.width *0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							   pFont->Draw("Ammo: ", { screenSize.width *0.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
							   pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(514.0f, 228.0f), SGD::Size(228.0f, 82.0f)));
							
							   for (size_t i = 0; i < 7; i++)
								   pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });
					
							   if (autoShotgunUpgrade.magSize.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.recoilTime.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.reloadTime.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.bulletSpread.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.damage.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.ammoCap.isMaxed == false)
								   pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.totalAmmo.isMaxed == false)
								   pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.isBought == false)
								   pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 
					 }
						 break;

					 }
					
					

	}
		break;
	case SMGS:
	{

				 pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
				 pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
				 pGraphics->DrawTexture(upgradeButton, { shotTab3.left, shotTab3.top }, {}, {}, {}, { 0.5f, 0.5f });
				 pFont->Draw("MAC-10", { shotTab1.left + 30, shotTab1.top + 5 }, 0.5f, { 255, 255, 0, 0 });
				 pFont->Draw("Tech-9mm", { shotTab2.left + 15, shotTab2.top + 5 }, 0.5f, { 255, 255, 0, 0 });
				 pFont->Draw("P90", { shotTab3.left + 45, shotTab3.top + 5 }, 0.5f, { 255, 255, 0, 0 });

				
				

				 switch (currTab)
				 {
				 case 0:
				 {
						   //UZI
						   string uziMagLevel = "LeveL ";
						   uziMagLevel += std::to_string(uziUpgrade.magSize.upgradedSkill.currTier);
						   uziMagLevel += " ";

						   string uziReloadLevel = "LeveL ";
						   uziReloadLevel += std::to_string(uziUpgrade.reloadTime.upgradedSkill.currTier);
						   uziReloadLevel += " ";

						   string uziBullSpreadLevel = "LeveL ";
						   uziBullSpreadLevel += std::to_string(uziUpgrade.bulletSpread.upgradedSkill.currTier);
						   uziBullSpreadLevel += " ";

						   string uziAmmoCapLevel = "LeveL ";
						   uziAmmoCapLevel += std::to_string(uziUpgrade.ammoCap.upgradedSkill.currTier);
						   uziAmmoCapLevel += " ";

						   string uziDamageLevel = "LeveL ";
						   uziDamageLevel += std::to_string(uziUpgrade.damage.upgradedSkill.currTier);
						   uziDamageLevel += " ";

						   pFont->Draw("MAC-10: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
						   pFont->Draw(uziMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(uziReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(uziBullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
						   pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(uziDamageLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(uziAmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });

						   pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(725.0f, 65.0f), SGD::Size(125.0f, 110.0f)));
						 
						   for (size_t i = 0; i < 6; i++)
							   pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });
						   pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });



						   if (uziUpgrade.magSize.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.reloadTime.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.bulletSpread.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.damage.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.ammoCap.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.totalAmmo.isMaxed == false)
							   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.isBought == false)
							   pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						  


				 }
					 break;
				 case 1:
				 {
						   //Tech9
						   string tech9MagLevel = "LeveL ";
						   tech9MagLevel += std::to_string(tech9Upgrade.magSize.upgradedSkill.currTier);
						   tech9MagLevel += " ";

						   string tech9ReloadLevel = "LeveL ";
						   tech9ReloadLevel += std::to_string(tech9Upgrade.reloadTime.upgradedSkill.currTier);
						   tech9ReloadLevel += " ";

						   string tech9BullSpreadLevel = "LeveL ";
						   tech9BullSpreadLevel += std::to_string(tech9Upgrade.bulletSpread.upgradedSkill.currTier);
						   tech9BullSpreadLevel += " ";

						   string tech9AmmoCapLevel = "LeveL ";
						   tech9AmmoCapLevel += std::to_string(tech9Upgrade.ammoCap.upgradedSkill.currTier);
						   tech9AmmoCapLevel += " ";

						   string tech9DamageLevel = "LeveL ";
						   tech9DamageLevel += std::to_string(tech9Upgrade.damage.upgradedSkill.currTier);
						   tech9DamageLevel += " ";

						   pFont->Draw("Tech-9mm: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
						   pFont->Draw(tech9MagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(tech9ReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(tech9BullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
						   pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(tech9DamageLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(tech9AmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
						   pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(335.0f, 670.0f), SGD::Size(165.0f, 100.0f)));
						 
						   for (size_t i = 0; i < 6; i++)
							   pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

						   pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });


						   if (tech9Upgrade.magSize.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.reloadTime.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.bulletSpread.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.damage.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.ammoCap.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.totalAmmo.isMaxed == false)
							   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.isBought == false)
							   pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });

				 }
					 break;
				 case 2:
				 {
						   //P90
						   string p90MagLevel = "LeveL ";
						   p90MagLevel += std::to_string(p90Upgrade.magSize.upgradedSkill.currTier);
						   p90MagLevel += " ";

						   string p90ReloadLevel = "LeveL ";
						   p90ReloadLevel += std::to_string(p90Upgrade.reloadTime.upgradedSkill.currTier);
						   p90ReloadLevel += " ";

						   string p90BullSpreadLevel = "LeveL ";
						   p90BullSpreadLevel += std::to_string(p90Upgrade.bulletSpread.upgradedSkill.currTier);
						   p90BullSpreadLevel += " ";

						   string p90AmmoCapLevel = "LeveL ";
						   p90AmmoCapLevel += std::to_string(p90Upgrade.ammoCap.upgradedSkill.currTier);
						   p90AmmoCapLevel += " ";

						   string p90DamageLevel = "LeveL ";
						   p90DamageLevel += std::to_string(p90Upgrade.damage.upgradedSkill.currTier);
						   p90DamageLevel += " ";

						   pFont->Draw("P90: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
						   pFont->Draw(p90MagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(p90ReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(p90BullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
						   pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(p90DamageLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw(p90AmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
						   pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(315.0f, 130.0f), SGD::Size(240.0f, 90.0f)));
	
						   for (size_t i = 0; i < 6; i++)
							   pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

						   pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });


						   if (p90Upgrade.magSize.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.reloadTime.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.bulletSpread.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.damage.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.ammoCap.isMaxed == false)
							   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.totalAmmo.isMaxed == false)
							   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.isBought == false)
							   pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
				 }
					 break;
				
				 }

				


				
	}
		break;
	case ASSAULT_RIFLES:
	{
						   pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
						   pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
						   pGraphics->DrawTexture(upgradeButton, { shotTab3.left, shotTab3.top }, {}, {}, {}, { 0.5f, 0.5f });
						   pFont->Draw("AK-47", { shotTab1.left + 20, shotTab1.top + 5 }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("M16", { shotTab2.left + 20, shotTab2.top + 5 }, 0.5f, { 255, 255, 0, 0 });
						   pFont->Draw("LMG", { shotTab3.left + 45, shotTab3.top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   switch (currTab)
						   {
						   case 0:
						   {
									 string akMagLevel = "LeveL ";
									 akMagLevel += std::to_string(ak47Upgrade.magSize.upgradedSkill.currTier);
									 akMagLevel += " ";

									 string akReloadLevel = "LeveL ";
									 akReloadLevel += std::to_string(ak47Upgrade.reloadTime.upgradedSkill.currTier);
									 akReloadLevel += " ";

									 string akRecoilLevel = "LeveL ";
									 akRecoilLevel += std::to_string(ak47Upgrade.recoilTime.upgradedSkill.currTier);
									 akRecoilLevel += " ";

									 string akBullSpreadLevel = "LeveL ";
									 akBullSpreadLevel += std::to_string(ak47Upgrade.bulletSpread.upgradedSkill.currTier);
									 akBullSpreadLevel += " ";

									 string akAmmoCapLevel = "LeveL ";
									 akAmmoCapLevel += std::to_string(ak47Upgrade.ammoCap.upgradedSkill.currTier);
									 akAmmoCapLevel += " ";

									 string akDamageLevel = "LeveL ";
									 akDamageLevel += std::to_string(ak47Upgrade.damage.upgradedSkill.currTier);
									 akDamageLevel += " ";

									 pFont->Draw("AK-47: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
									 pFont->Draw(akMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(akReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(akRecoilLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Rate of Fire: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(akBullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5, { 255, 255, 0, 0 });
									 pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(akDamageLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(akAmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
									 pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(225.0f, 300.0f), SGD::Size(244.0f, 73.0f)));
									
									 for (size_t i = 0; i < 7; i++)
										 pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

									 pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });


									 if (ak47Upgrade.magSize.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.reloadTime.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.recoilTime.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.bulletSpread.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.damage.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.ammoCap.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.totalAmmo.isMaxed == false)
										 pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.isBought == false)
										 pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   }
							   break;
						
						   case 1:
						   {
									 string m16MagLevel = "LeveL ";
									 m16MagLevel += std::to_string(m16Upgrade.magSize.upgradedSkill.currTier);
									 m16MagLevel += " ";

									 string m16ReloadLevel = "LeveL ";
									 m16ReloadLevel += std::to_string(m16Upgrade.reloadTime.upgradedSkill.currTier);
									 m16ReloadLevel += " ";

									 string m16RecoilLevel = "LeveL ";
									 m16RecoilLevel += std::to_string(m16Upgrade.recoilTime.upgradedSkill.currTier);
									 m16RecoilLevel += " ";

									 string m16BullSpreadLevel = "LeveL ";
									 m16BullSpreadLevel += std::to_string(m16Upgrade.bulletSpread.upgradedSkill.currTier);
									 m16BullSpreadLevel += " ";

									 string m16AmmoCapLevel = "LeveL ";
									 m16AmmoCapLevel += std::to_string(m16Upgrade.ammoCap.upgradedSkill.currTier);
									 m16AmmoCapLevel += " ";

									 string m16DamageLevel = "LeveL ";
									 m16DamageLevel += std::to_string(m16Upgrade.damage.upgradedSkill.currTier);
									 m16DamageLevel += " ";

									 pFont->Draw("M-16: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
									 pFont->Draw(m16MagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(m16ReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(m16RecoilLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Rate of Fire: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(m16BullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5, { 255, 255, 0, 0 });
									 pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(m16DamageLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(m16AmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
									 pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(468.0f, 312.0f), SGD::Size(256.0f, 88.0f)));
									
									 for (size_t i = 0; i < 7; i++)
										 pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

									 pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });


									 if (m16Upgrade.magSize.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.reloadTime.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.recoilTime.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.bulletSpread.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.damage.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.ammoCap.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.totalAmmo.isMaxed == false)
										 pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.isBought == false)
										 pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });


						   }
							   break;
						   case 2:
						   {
									 string lmgMagLevel = "LeveL ";
									 lmgMagLevel += std::to_string(lmgUpgrade.magSize.upgradedSkill.currTier);
									 lmgMagLevel += " ";

									 string lmgReloadLevel = "LeveL ";
									 lmgReloadLevel += std::to_string(lmgUpgrade.reloadTime.upgradedSkill.currTier);
									 lmgReloadLevel += " ";

									 string lmgRecoilLevel = "LeveL ";
									 lmgRecoilLevel += std::to_string(lmgUpgrade.recoilTime.upgradedSkill.currTier);
									 lmgRecoilLevel += " ";

									 string lmgBullSpreadLevel = "LeveL ";
									 lmgBullSpreadLevel += std::to_string(lmgUpgrade.bulletSpread.upgradedSkill.currTier);
									 lmgBullSpreadLevel += " ";

									 string lmgAmmoCapLevel = "LeveL ";
									 lmgAmmoCapLevel += std::to_string(lmgUpgrade.ammoCap.upgradedSkill.currTier);
									 lmgAmmoCapLevel += " ";

									 string lmgDamageLevel = "LeveL ";
									 lmgDamageLevel += std::to_string(lmgUpgrade.damage.upgradedSkill.currTier);
									 lmgDamageLevel += " ";

									 pFont->Draw("LMG: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
									 pFont->Draw(lmgMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(lmgReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(lmgRecoilLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Rate of Fire: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(lmgBullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5, { 255, 255, 0, 0 });
									 pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(lmgDamageLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw(lmgAmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
									 pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
									 pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(16.0f, 368.0f), SGD::Size(260.0f, 85.0f)));

									 for (size_t i = 0; i < 7; i++)
										 pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

									 pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });


									 if (lmgUpgrade.magSize.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.reloadTime.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.recoilTime.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.bulletSpread.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.damage.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.ammoCap.isMaxed == false)
										 pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.totalAmmo.isMaxed == false)
										 pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.isBought == false)
										 pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   }
							   break;
						   }
						  
					
	}
		break;
	case HEAVY:
	{
				  pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
				  pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
				  pGraphics->DrawTexture(upgradeButton, { shotTab3.left, shotTab3.top }, {}, {}, {}, { 0.5f, 0.5f });
				  pFont->Draw("Sniper", { shotTab1.left + 20, shotTab1.top + 5 }, 0.5f, { 255, 255, 0, 0 });
				  pFont->Draw("F. Thrower", { shotTab2.left + 20, shotTab2.top + 5 }, 0.5f, { 255, 255, 0, 0 });
				  pFont->Draw("G.Launcher", { shotTab3.left + 45, shotTab3.top + 5 }, 0.5f, { 255, 255, 0, 0 });
				  //Revolver
				  switch (currTab)
				  {
				  case 0:
				  {
							string sniperMagLevel = "LeveL ";
							sniperMagLevel += std::to_string(sniperUpgrade.magSize.upgradedSkill.currTier);
							sniperMagLevel += " ";

							string sniperReloadLevel = "LeveL ";
							sniperReloadLevel += std::to_string(sniperUpgrade.reloadTime.upgradedSkill.currTier);
							sniperReloadLevel += " ";

							string sniperRecoilLevel = "LeveL ";
							sniperRecoilLevel += std::to_string(sniperUpgrade.recoilTime.upgradedSkill.currTier);
							sniperRecoilLevel += " ";

							string sniperBullSpreadLevel = "LeveL ";
							sniperBullSpreadLevel += std::to_string(sniperUpgrade.bulletSpread.upgradedSkill.currTier);
							sniperBullSpreadLevel += " ";

							string sniperAmmoCapLevel = "LeveL ";
							sniperAmmoCapLevel += std::to_string(sniperUpgrade.ammoCap.upgradedSkill.currTier);
							sniperAmmoCapLevel += " ";

							string sniperPenPowerLevel = "LeveL ";
							sniperPenPowerLevel += std::to_string(sniperUpgrade.penPower.upgradedSkill.currTier);
							sniperPenPowerLevel += " ";

							string sniperDamageLevel = "LeveL ";
							sniperDamageLevel += std::to_string(sniperUpgrade.damage.upgradedSkill.currTier);
							sniperDamageLevel += " ";

							pFont->Draw("Sniper Rifler: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
							pFont->Draw(sniperMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(sniperReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(sniperRecoilLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Rate of Fire: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(sniperBullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5, { 255, 255, 0, 0 });
							pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(sniperDamageLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(sniperPenPowerLevel.c_str(), { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Penetration Power: ", { screenSize.width * 0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(sniperAmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[7].top }, 0.5f, { 255, 255, 0, 0 });
							pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(530.0f, 400.0f), SGD::Size(230.0f, 63.0f)));

							for (size_t i = 0; i < 8; i++)
								pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

							pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });

							
							if (sniperUpgrade.magSize.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.reloadTime.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.recoilTime.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.bulletSpread.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.damage.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.penPower.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.ammoCap.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[6].left + 5, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[6].left + 25, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.totalAmmo.isMaxed == false)
								pFont->Draw("Buy Ammo", { Buttons[7].left + 18, Buttons[7].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Ammo Full", { Buttons[7].left + 18, Buttons[7].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.isBought == false)
								pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
				  }
					  break;
				  case 1:
				  {
							string flameMagLevel = "LeveL ";
							flameMagLevel += std::to_string(flameUpgrade.magSize.upgradedSkill.currTier);
							flameMagLevel += " ";

							string flameReloadLevel = "LeveL ";
							flameReloadLevel += std::to_string(flameUpgrade.reloadTime.upgradedSkill.currTier);
							flameReloadLevel += " ";

							string flameBullSpreadLevel = "LeveL ";
							flameBullSpreadLevel += std::to_string(flameUpgrade.bulletSpread.upgradedSkill.currTier);
							flameBullSpreadLevel += " ";

							string flameAmmoCapLevel = "LeveL ";
							flameAmmoCapLevel += std::to_string(flameUpgrade.ammoCap.upgradedSkill.currTier);
							flameAmmoCapLevel += " ";

							string flameDamageLevel = "LeveL ";
							flameDamageLevel += std::to_string(flameUpgrade.damage.upgradedSkill.currTier);
							flameDamageLevel += " ";

							string flameRangeLevel = "LeveL ";
							flameRangeLevel += std::to_string(flameUpgrade.bulletVelocity.upgradedSkill.currTier);
							flameRangeLevel += " ";

							pFont->Draw("Flame Thrower: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
							pFont->Draw(flameMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Fuel Tank Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(flameReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(flameBullSpreadLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5, { 255, 255, 0, 0 });
							pFont->Draw("Stability: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(flameDamageLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(flameRangeLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Range: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(flameAmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[6].top }, 0.5f, { 255, 255, 0, 0 });
							pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(8.0f, 550.0f), SGD::Size(274.0f, 142.0f)));
							
							for (size_t i = 0; i < 7; i++)
								pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

							pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });
							
							if (flameUpgrade.magSize.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.reloadTime.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							
							if (flameUpgrade.bulletSpread.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.damage.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.bulletVelocity.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							
							if (flameUpgrade.ammoCap.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.totalAmmo.isMaxed == false)
								pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.isBought == false)
								pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
							
				  }
					  break;
				  case 2:
				  {
							string nadeMagLevel = "LeveL ";
							nadeMagLevel += std::to_string(nadeLauncherUpgrade.magSize.upgradedSkill.currTier);
							nadeMagLevel += " ";

							string nadeReloadLevel = "LeveL ";
							nadeReloadLevel += std::to_string(nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier);
							nadeReloadLevel += " ";

							string nadeAmmoCapLevel = "LeveL ";
							nadeAmmoCapLevel += std::to_string(nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier);
							nadeAmmoCapLevel += " ";

							string nadeDamageLevel = "LeveL ";
							nadeDamageLevel += std::to_string(nadeLauncherUpgrade.damage.upgradedSkill.currTier);
							nadeDamageLevel += " ";

							string nadeRangeLevel = "LeveL ";
							nadeRangeLevel += std::to_string(nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier);
							nadeRangeLevel += " ";

							pFont->Draw("Grenade Launcher: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
							pFont->Draw(nadeMagLevel.c_str(), { screenSize.width *.1f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Magezine Size: ", { screenSize.width * 0.3f, Buttons[0].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(nadeReloadLevel.c_str(), { screenSize.width *.1f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Reload Speed: ", { screenSize.width * 0.3f, Buttons[1].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(nadeDamageLevel.c_str(), { screenSize.width *.1f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Damage: ", { screenSize.width * 0.3f, Buttons[2].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(nadeRangeLevel.c_str(), { screenSize.width *.1f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Range: ", { screenSize.width * 0.3f, Buttons[3].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw(nadeAmmoCapLevel.c_str(), { screenSize.width *.1f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Ammo Capacity: ", { screenSize.width * 0.3f, Buttons[4].top }, 0.5f, { 255, 255, 0, 0 });
							pFont->Draw("Ammo: ", { screenSize.width *.1f, Buttons[5].top }, 0.5f, { 255, 255, 0, 0 });
							pGraphics->DrawTextureSection(weaponsImage, { screenSize.width * .70f, screenSize.height * 0.45f }, SGD::Rectangle(SGD::Point(330.0f, 455.0f), SGD::Size(265.0f, 82.0f)));
				
							for (size_t i = 0; i < 6; i++)
								pGraphics->DrawTexture(upgradeButton, { Buttons[i].left, Buttons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

							pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });

							if (nadeLauncherUpgrade.magSize.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.reloadTime.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.damage.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.bulletVelocity.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

								if (nadeLauncherUpgrade.ammoCap.isMaxed == false)
								pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.totalAmmo.isMaxed == false)
								pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.isBought == false)
								pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });

				  }
					  break;
				  }
				 

	}
		break;
	case DEFENSE:
	
					/*pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
					pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
					pGraphics->DrawTexture(upgradeButton, { shotTab3.left, shotTab3.top }, {}, {}, {}, { 0.5f, 0.5f });
					pFont->Draw("Sniper", { shotTab1.left + 20, shotTab1.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw("F. Thrower", { shotTab2.left + 20, shotTab2.top + 5 }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw("G.Launcher", { shotTab3.left + 45, shotTab3.top + 5 }, 0.5f, { 255, 255, 0, 0 });*/

					string sandbagStatus = "";
					sandbagStatus += std::to_string(sandBagCurrHealth);
					sandbagStatus += " ";

					string sandbagMaxHealth = "";
					sandbagMaxHealth += std::to_string(sandBagMaxHealth);
					sandbagMaxHealth += " ";

					string barbedDamageStatus = "";
					barbedDamageStatus += std::to_string(profile.barbWire.damage.upgradedSkill.stat);
					barbedDamageStatus += " ";

					string barbedHeatlhStatus = "";
					barbedHeatlhStatus += std::to_string(barbWireCurrHealth);
					barbedHeatlhStatus += " ";

					string barbedMaxHealth = "";
					barbedMaxHealth += std::to_string(barbWireMaxHealth);
					barbedMaxHealth += " ";

					string landmineStatus = "LeveL ";
					landmineStatus += std::to_string(numLandMines);
					landmineStatus += " ";

					string turretInventory = "LeveL ";
					turretInventory += std::to_string(profile.numTurrets);
					turretInventory += " ";


					pFont->Draw("Sandbags: ", { screenSize.width *.1f, screenSize.height * 0.1f }, 1.0f, { 255, 255, 0, 0 });
					pFont->Draw("Durability: ", { screenSize.width * 0.1f, DefenseButtons[0].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(sandbagStatus.c_str(), { screenSize.width *.3f, DefenseButtons[0].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw("Max Durability: ", { screenSize.width * 0.1f, DefenseButtons[1].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(sandbagMaxHealth.c_str(), { screenSize.width *.3f, DefenseButtons[1].top }, 0.5f, { 255, 255, 0, 0 });

					pFont->Draw("Barbed Wire: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
					pFont->Draw("Durability: ", { screenSize.width * 0.1f, DefenseButtons[2].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(barbedHeatlhStatus.c_str(), { screenSize.width *.3f, DefenseButtons[2].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw("Max Durability: ", { screenSize.width * 0.1f, DefenseButtons[3].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(barbedMaxHealth.c_str(), { screenSize.width *.3f, DefenseButtons[3].top }, 0.5f, { 255, 255, 0, 0 });

					pFont->Draw("Damage: ", { screenSize.width * 0.1f, DefenseButtons[4].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(barbedDamageStatus.c_str(), { screenSize.width *.3f, DefenseButtons[4].top }, 0.5f, { 255, 255, 0, 0 });
					

					pFont->Draw("Mine Field: ", { screenSize.width *.1f, screenSize.height * 0.5f }, 1.0f, { 255, 255, 0, 0 });
					pFont->Draw("Durability: ", { screenSize.width * 0.1f, DefenseButtons[5].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(landmineStatus.c_str(), { screenSize.width *.3f, DefenseButtons[5].top }, 0.5f, { 255, 255, 0, 0 });

					pFont->Draw("Auto-Turrets: ", { screenSize.width *.1f, screenSize.height * 0.7f }, 1.0f, { 255, 255, 0, 0 });
					pFont->Draw("Inventory: ", { screenSize.width * 0.1f, DefenseButtons[6].top }, 0.5f, { 255, 255, 0, 0 });
					pFont->Draw(turretInventory.c_str(), { screenSize.width *.3f, DefenseButtons[6].top }, 0.5f, { 255, 255, 0, 0 });


					for (size_t i = 0; i < 7; i++)
						pGraphics->DrawTexture(upgradeButton, { DefenseButtons[i].left, DefenseButtons[i].top }, {}, {}, {}, { 0.5f, 0.5f });

				
					if (sandBag.isBought == true)
					{
						if (sandBagCurrHealth < sandBagMaxHealth)
							pFont->Draw("Repair", { DefenseButtons[0].left + 5, DefenseButtons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { DefenseButtons[0].left + 25, DefenseButtons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });
						
						if (sandBag.maxHealth.isMaxed == false)
							pFont->Draw("Upgrade", { DefenseButtons[1].left + 5, DefenseButtons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { DefenseButtons[1].left + 25, DefenseButtons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					}
					else
						pFont->Draw("Buy", { DefenseButtons[0].left + 25, DefenseButtons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });



					if (barbedwire.isBought == true)
					{
						if (barbWireCurrHealth < barbWireMaxHealth)
							pFont->Draw("Repair", { DefenseButtons[2].left + 5, DefenseButtons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { DefenseButtons[2].left + 25, DefenseButtons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (barbedwire.maxHealth.isMaxed == false)
							pFont->Draw("Upgrade", { DefenseButtons[3].left + 5, DefenseButtons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { DefenseButtons[3].left + 25, DefenseButtons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 }); 

						if (barbedwire.damage.isMaxed == false)
							pFont->Draw("Upgrade", { DefenseButtons[4].left + 5, DefenseButtons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { DefenseButtons[4].left + 25, DefenseButtons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					}
					else
						pFont->Draw("Buy", { DefenseButtons[2].left + 25, DefenseButtons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });




					if (landMine.isBought == true)
					{
						if (numLandMines < landMines.size())
							pFont->Draw("Repair", { DefenseButtons[5].left + 5, DefenseButtons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { DefenseButtons[5].left + 25, DefenseButtons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					}
					else
						pFont->Draw("Buy", { DefenseButtons[5].left + 25, DefenseButtons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

					
					pFont->Draw("Buy", { DefenseButtons[6].left + 25, DefenseButtons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });


						
						/*if (sniperUpgrade.reloadTime.isMaxed == false)
						pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.recoilTime.isMaxed == false)
						pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.bulletSpread.isMaxed == false)
						pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.damage.isMaxed == false)
						pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.penPower.isMaxed == false)
						pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.ammoCap.isMaxed == false)
						pFont->Draw("Upgrade", { Buttons[6].left + 5, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Maxed", { Buttons[6].left + 25, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.totalAmmo.isMaxed == false)
						pFont->Draw("Buy Ammo", { Buttons[7].left + 18, Buttons[7].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Ammo Full", { Buttons[7].left + 18, Buttons[7].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (sniperUpgrade.isBought == false)
						pFont->Draw("Buy Gun", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
						pFont->Draw("Equipt", { Buttons[8].left + 18, Buttons[8].top + 5 }, 0.5f, { 255, 255, 0, 0 });
						*/
						break;
						}
	
	// Draw the reticle
	SGD::Point	retpos = SGD::InputManager::GetInstance()->GetMousePosition();
	//float		retscale = 0.8f;

	retpos.Offset(-5.0f, -5.0f);
	pGraphics->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { 1.0f, 1.0f });

}

void ShopState::LoadShopStatus()
{
	GamerProfile profile = Game::GetInstance()->GetProfile();

#pragma region Pistol
	pistolUpgrade.magSize.upgradedSkill.currTier = profile.pistol.magSize.upgradedSkill.currTier;
	pistolUpgrade.magSize.upgradedSkill.maxTier = profile.pistol.magSize.upgradedSkill.maxTier;
	pistolUpgrade.magSize.upgradedSkill.stat = profile.pistol.magSize.upgradedSkill.stat;

	pistolUpgrade.recoilTime.upgradedSkill.currTier = profile.pistol.recoilTime.upgradedSkill.currTier;
	pistolUpgrade.recoilTime.upgradedSkill.maxTier = profile.pistol.recoilTime.upgradedSkill.maxTier;
	pistolUpgrade.recoilTime.upgradedSkill.stat = profile.pistol.recoilTime.upgradedSkill.stat;

	pistolUpgrade.reloadTime.upgradedSkill.currTier = profile.pistol.reloadTime.upgradedSkill.currTier;
	pistolUpgrade.reloadTime.upgradedSkill.maxTier = profile.pistol.reloadTime.upgradedSkill.maxTier;
	pistolUpgrade.reloadTime.upgradedSkill.stat = profile.pistol.reloadTime.upgradedSkill.stat;


#pragma endregion

#pragma region Revoler Loadin

	revolverUpgrade.magSize.upgradedSkill.currTier = profile.revolver.reloadTime.upgradedSkill.currTier;
	revolverUpgrade.magSize.upgradedSkill.maxTier = profile.revolver.reloadTime.upgradedSkill.maxTier;
	revolverUpgrade.magSize.upgradedSkill.stat = profile.revolver.reloadTime.upgradedSkill.stat;

	revolverUpgrade.reloadTime.upgradedSkill.currTier = profile.revolver.reloadTime.upgradedSkill.currTier;
	revolverUpgrade.reloadTime.upgradedSkill.maxTier = profile.revolver.reloadTime.upgradedSkill.maxTier;
	revolverUpgrade.reloadTime.upgradedSkill.stat = profile.revolver.reloadTime.upgradedSkill.stat;

	revolverUpgrade.recoilTime.upgradedSkill.currTier = profile.revolver.reloadTime.upgradedSkill.currTier;
	revolverUpgrade.recoilTime.upgradedSkill.maxTier = profile.revolver.reloadTime.upgradedSkill.maxTier;
	revolverUpgrade.recoilTime.upgradedSkill.stat = profile.revolver.reloadTime.upgradedSkill.stat;
	
	revolverUpgrade.penPower.upgradedSkill.currTier = profile.revolver.penPower.upgradedSkill.currTier;
	revolverUpgrade.penPower.upgradedSkill.maxTier = profile.revolver.penPower.upgradedSkill.maxTier;
	revolverUpgrade.penPower.upgradedSkill.stat = profile.revolver.penPower.upgradedSkill.stat;

	revolverUpgrade.damage.upgradedSkill.currTier = profile.revolver.damage.upgradedSkill.currTier;
	revolverUpgrade.damage.upgradedSkill.maxTier = profile.revolver.damage.upgradedSkill.maxTier;
	revolverUpgrade.damage.upgradedSkill.stat = profile.revolver.damage.upgradedSkill.stat;

	revolverUpgrade.ammoCap.upgradedSkill.currTier = profile.revolver.ammoCap.upgradedSkill.currTier;
	revolverUpgrade.ammoCap.upgradedSkill.maxTier = profile.revolver.ammoCap.upgradedSkill.maxTier;
	revolverUpgrade.ammoCap.upgradedSkill.stat = profile.revolver.ammoCap.upgradedSkill.stat;

	revolverUpgrade.totalAmmo.upgradedSkill.currTier = profile.revolver.totalAmmo.upgradedSkill.currTier;
	revolverUpgrade.totalAmmo.upgradedSkill.maxTier = profile.revolver.totalAmmo.upgradedSkill.maxTier;
	revolverUpgrade.totalAmmo.upgradedSkill.stat = profile.revolver.totalAmmo.upgradedSkill.stat;

	revolverUpgrade.isBought = profile.revolver.isBought;

#pragma endregion


#pragma region Sawnoff Loadin

	

	sawnOffUpgrade.reloadTime.upgradedSkill.currTier = profile.sawnoff.reloadTime.upgradedSkill.currTier;
	sawnOffUpgrade.reloadTime.upgradedSkill.maxTier = profile.sawnoff.reloadTime.upgradedSkill.maxTier;
	sawnOffUpgrade.reloadTime.upgradedSkill.stat = profile.sawnoff.reloadTime.upgradedSkill.stat;

	sawnOffUpgrade.recoilTime.upgradedSkill.currTier = profile.sawnoff.reloadTime.upgradedSkill.currTier;
	sawnOffUpgrade.recoilTime.upgradedSkill.maxTier = profile.sawnoff.reloadTime.upgradedSkill.maxTier;
	sawnOffUpgrade.recoilTime.upgradedSkill.stat = profile.sawnoff.reloadTime.upgradedSkill.stat;

	sawnOffUpgrade.bulletSpread.upgradedSkill.currTier = profile.sawnoff.reloadTime.upgradedSkill.currTier;
	sawnOffUpgrade.bulletSpread.upgradedSkill.maxTier = profile.sawnoff.reloadTime.upgradedSkill.maxTier;
	sawnOffUpgrade.bulletSpread.upgradedSkill.stat = profile.sawnoff.reloadTime.upgradedSkill.stat;
	
	sawnOffUpgrade.damage.upgradedSkill.currTier = profile.sawnoff.damage.upgradedSkill.currTier;
	sawnOffUpgrade.damage.upgradedSkill.maxTier = profile.sawnoff.damage.upgradedSkill.maxTier;
	sawnOffUpgrade.damage.upgradedSkill.stat = profile.sawnoff.damage.upgradedSkill.stat;

	sawnOffUpgrade.ammoCap.upgradedSkill.currTier = profile.sawnoff.ammoCap.upgradedSkill.currTier;
	sawnOffUpgrade.ammoCap.upgradedSkill.maxTier = profile.sawnoff.ammoCap.upgradedSkill.maxTier;
	sawnOffUpgrade.ammoCap.upgradedSkill.stat = profile.sawnoff.ammoCap.upgradedSkill.stat;

	sawnOffUpgrade.totalAmmo.upgradedSkill.currTier = profile.sawnoff.totalAmmo.upgradedSkill.currTier;
	sawnOffUpgrade.totalAmmo.upgradedSkill.maxTier = profile.sawnoff.totalAmmo.upgradedSkill.maxTier;
	sawnOffUpgrade.totalAmmo.upgradedSkill.stat = profile.sawnoff.totalAmmo.upgradedSkill.stat;

	sawnOffUpgrade.isBought = profile.sawnoff.isBought;

#pragma endregion


#pragma region PumpAction Loadin

	pumpShotgunUpgrade.magSize.upgradedSkill.currTier = profile.pumpShotgun.magSize.upgradedSkill.currTier;
	pumpShotgunUpgrade.magSize.upgradedSkill.maxTier = profile.pumpShotgun.magSize.upgradedSkill.maxTier;
	pumpShotgunUpgrade.magSize.upgradedSkill.stat = profile.pumpShotgun.magSize.upgradedSkill.stat;

	pumpShotgunUpgrade.reloadTime.upgradedSkill.currTier = profile.pumpShotgun.reloadTime.upgradedSkill.currTier;
	pumpShotgunUpgrade.reloadTime.upgradedSkill.maxTier = profile.pumpShotgun.reloadTime.upgradedSkill.maxTier;
	pumpShotgunUpgrade.reloadTime.upgradedSkill.stat = profile.pumpShotgun.reloadTime.upgradedSkill.stat;

	pumpShotgunUpgrade.recoilTime.upgradedSkill.currTier = profile.pumpShotgun.reloadTime.upgradedSkill.currTier;
	pumpShotgunUpgrade.recoilTime.upgradedSkill.maxTier = profile.pumpShotgun.reloadTime.upgradedSkill.maxTier;
	pumpShotgunUpgrade.recoilTime.upgradedSkill.stat = profile.pumpShotgun.reloadTime.upgradedSkill.stat;

	pumpShotgunUpgrade.bulletSpread.upgradedSkill.currTier = profile.pumpShotgun.reloadTime.upgradedSkill.currTier;
	pumpShotgunUpgrade.bulletSpread.upgradedSkill.maxTier = profile.pumpShotgun.reloadTime.upgradedSkill.maxTier;
	pumpShotgunUpgrade.bulletSpread.upgradedSkill.stat = profile.pumpShotgun.reloadTime.upgradedSkill.stat;

	pumpShotgunUpgrade.damage.upgradedSkill.currTier = profile.pumpShotgun.damage.upgradedSkill.currTier;
	pumpShotgunUpgrade.damage.upgradedSkill.maxTier = profile.pumpShotgun.damage.upgradedSkill.maxTier;
	pumpShotgunUpgrade.damage.upgradedSkill.stat = profile.pumpShotgun.damage.upgradedSkill.stat;

	pumpShotgunUpgrade.ammoCap.upgradedSkill.currTier = profile.pumpShotgun.ammoCap.upgradedSkill.currTier;
	pumpShotgunUpgrade.ammoCap.upgradedSkill.maxTier = profile.pumpShotgun.ammoCap.upgradedSkill.maxTier;
	pumpShotgunUpgrade.ammoCap.upgradedSkill.stat = profile.pumpShotgun.ammoCap.upgradedSkill.stat;

	pumpShotgunUpgrade.totalAmmo.upgradedSkill.currTier = profile.pumpShotgun.totalAmmo.upgradedSkill.currTier;
	pumpShotgunUpgrade.totalAmmo.upgradedSkill.maxTier = profile.pumpShotgun.totalAmmo.upgradedSkill.maxTier;
	pumpShotgunUpgrade.totalAmmo.upgradedSkill.stat = profile.pumpShotgun.totalAmmo.upgradedSkill.stat;

	pumpShotgunUpgrade.isBought = profile.pumpShotgun.isBought;

#pragma endregion


#pragma region AutoShotty Loadin

	autoShotgunUpgrade.magSize.upgradedSkill.currTier = profile.autoShotgun.magSize.upgradedSkill.currTier;
	autoShotgunUpgrade.magSize.upgradedSkill.maxTier = profile.autoShotgun.magSize.upgradedSkill.maxTier;
	autoShotgunUpgrade.magSize.upgradedSkill.stat = profile.autoShotgun.magSize.upgradedSkill.stat;

	autoShotgunUpgrade.reloadTime.upgradedSkill.currTier = profile.autoShotgun.reloadTime.upgradedSkill.currTier;
	autoShotgunUpgrade.reloadTime.upgradedSkill.maxTier = profile.autoShotgun.reloadTime.upgradedSkill.maxTier;
	autoShotgunUpgrade.reloadTime.upgradedSkill.stat = profile.autoShotgun.reloadTime.upgradedSkill.stat;

	autoShotgunUpgrade.recoilTime.upgradedSkill.currTier = profile.autoShotgun.reloadTime.upgradedSkill.currTier;
	autoShotgunUpgrade.recoilTime.upgradedSkill.maxTier = profile.autoShotgun.reloadTime.upgradedSkill.maxTier;
	autoShotgunUpgrade.recoilTime.upgradedSkill.stat = profile.autoShotgun.reloadTime.upgradedSkill.stat;

	autoShotgunUpgrade.bulletSpread.upgradedSkill.currTier = profile.autoShotgun.reloadTime.upgradedSkill.currTier;
	autoShotgunUpgrade.bulletSpread.upgradedSkill.maxTier = profile.autoShotgun.reloadTime.upgradedSkill.maxTier;
	autoShotgunUpgrade.bulletSpread.upgradedSkill.stat = profile.autoShotgun.reloadTime.upgradedSkill.stat;

	autoShotgunUpgrade.damage.upgradedSkill.currTier = profile.autoShotgun.damage.upgradedSkill.currTier;
	autoShotgunUpgrade.damage.upgradedSkill.maxTier = profile.autoShotgun.damage.upgradedSkill.maxTier;
	autoShotgunUpgrade.damage.upgradedSkill.stat = profile.autoShotgun.damage.upgradedSkill.stat;

	autoShotgunUpgrade.ammoCap.upgradedSkill.currTier = profile.autoShotgun.ammoCap.upgradedSkill.currTier;
	autoShotgunUpgrade.ammoCap.upgradedSkill.maxTier = profile.autoShotgun.ammoCap.upgradedSkill.maxTier;
	autoShotgunUpgrade.ammoCap.upgradedSkill.stat = profile.autoShotgun.ammoCap.upgradedSkill.stat;

	autoShotgunUpgrade.totalAmmo.upgradedSkill.currTier = profile.autoShotgun.totalAmmo.upgradedSkill.currTier;
	autoShotgunUpgrade.totalAmmo.upgradedSkill.maxTier = profile.autoShotgun.totalAmmo.upgradedSkill.maxTier;
	autoShotgunUpgrade.totalAmmo.upgradedSkill.stat = profile.autoShotgun.totalAmmo.upgradedSkill.stat;

	autoShotgunUpgrade.isBought = profile.autoShotgun.isBought;

#pragma endregion

#pragma region UZI Loadin

	uziUpgrade.magSize.upgradedSkill.currTier = profile.mac10.magSize.upgradedSkill.currTier;
	uziUpgrade.magSize.upgradedSkill.maxTier = profile.mac10.magSize.upgradedSkill.maxTier;
	uziUpgrade.magSize.upgradedSkill.stat = profile.mac10.magSize.upgradedSkill.stat;

	uziUpgrade.reloadTime.upgradedSkill.currTier = profile.mac10.reloadTime.upgradedSkill.currTier;
	uziUpgrade.reloadTime.upgradedSkill.maxTier = profile.mac10.reloadTime.upgradedSkill.maxTier;
	uziUpgrade.reloadTime.upgradedSkill.stat = profile.mac10.reloadTime.upgradedSkill.stat;

	
	uziUpgrade.bulletSpread.upgradedSkill.currTier = profile.mac10.reloadTime.upgradedSkill.currTier;
	uziUpgrade.bulletSpread.upgradedSkill.maxTier = profile.mac10.reloadTime.upgradedSkill.maxTier;
	uziUpgrade.bulletSpread.upgradedSkill.stat = profile.mac10.reloadTime.upgradedSkill.stat;

	uziUpgrade.damage.upgradedSkill.currTier = profile.mac10.damage.upgradedSkill.currTier;
	uziUpgrade.damage.upgradedSkill.maxTier = profile.mac10.damage.upgradedSkill.maxTier;
	uziUpgrade.damage.upgradedSkill.stat = profile.mac10.damage.upgradedSkill.stat;

	uziUpgrade.ammoCap.upgradedSkill.currTier = profile.mac10.ammoCap.upgradedSkill.currTier;
	uziUpgrade.ammoCap.upgradedSkill.maxTier = profile.mac10.ammoCap.upgradedSkill.maxTier;
	uziUpgrade.ammoCap.upgradedSkill.stat = profile.mac10.ammoCap.upgradedSkill.stat;

	uziUpgrade.totalAmmo.upgradedSkill.currTier = profile.mac10.totalAmmo.upgradedSkill.currTier;
	uziUpgrade.totalAmmo.upgradedSkill.maxTier = profile.mac10.totalAmmo.upgradedSkill.maxTier;
	uziUpgrade.totalAmmo.upgradedSkill.stat = profile.mac10.totalAmmo.upgradedSkill.stat;

	uziUpgrade.isBought = profile.mac10.isBought;

#pragma endregion

#pragma region Tech9mm Loadin

	tech9Upgrade.magSize.upgradedSkill.currTier = profile.tech9.magSize.upgradedSkill.currTier;
	tech9Upgrade.magSize.upgradedSkill.maxTier = profile.tech9.magSize.upgradedSkill.maxTier;
	tech9Upgrade.magSize.upgradedSkill.stat = profile.tech9.magSize.upgradedSkill.stat;

	tech9Upgrade.reloadTime.upgradedSkill.currTier = profile.tech9.reloadTime.upgradedSkill.currTier;
	tech9Upgrade.reloadTime.upgradedSkill.maxTier = profile.tech9.reloadTime.upgradedSkill.maxTier;
	tech9Upgrade.reloadTime.upgradedSkill.stat = profile.tech9.reloadTime.upgradedSkill.stat;


	tech9Upgrade.bulletSpread.upgradedSkill.currTier = profile.tech9.reloadTime.upgradedSkill.currTier;
	tech9Upgrade.bulletSpread.upgradedSkill.maxTier = profile.tech9.reloadTime.upgradedSkill.maxTier;
	tech9Upgrade.bulletSpread.upgradedSkill.stat = profile.tech9.reloadTime.upgradedSkill.stat;

	tech9Upgrade.damage.upgradedSkill.currTier = profile.tech9.damage.upgradedSkill.currTier;
	tech9Upgrade.damage.upgradedSkill.maxTier = profile.tech9.damage.upgradedSkill.maxTier;
	tech9Upgrade.damage.upgradedSkill.stat = profile.tech9.damage.upgradedSkill.stat;

	tech9Upgrade.ammoCap.upgradedSkill.currTier = profile.tech9.ammoCap.upgradedSkill.currTier;
	tech9Upgrade.ammoCap.upgradedSkill.maxTier = profile.tech9.ammoCap.upgradedSkill.maxTier;
	tech9Upgrade.ammoCap.upgradedSkill.stat = profile.tech9.ammoCap.upgradedSkill.stat;

	tech9Upgrade.totalAmmo.upgradedSkill.currTier = profile.tech9.totalAmmo.upgradedSkill.currTier;
	tech9Upgrade.totalAmmo.upgradedSkill.maxTier = profile.tech9.totalAmmo.upgradedSkill.maxTier;
	tech9Upgrade.totalAmmo.upgradedSkill.stat = profile.tech9.totalAmmo.upgradedSkill.stat;

	tech9Upgrade.isBought = profile.tech9.isBought;

#pragma endregion

#pragma region P90 Loadin

	p90Upgrade.magSize.upgradedSkill.currTier = profile.p90.magSize.upgradedSkill.currTier;
	p90Upgrade.magSize.upgradedSkill.maxTier = profile.p90.magSize.upgradedSkill.maxTier;
	p90Upgrade.magSize.upgradedSkill.stat = profile.p90.magSize.upgradedSkill.stat;

	p90Upgrade.reloadTime.upgradedSkill.currTier = profile.p90.reloadTime.upgradedSkill.currTier;
	p90Upgrade.reloadTime.upgradedSkill.maxTier = profile.p90.reloadTime.upgradedSkill.maxTier;
	p90Upgrade.reloadTime.upgradedSkill.stat = profile.p90.reloadTime.upgradedSkill.stat;


	p90Upgrade.bulletSpread.upgradedSkill.currTier = profile.p90.reloadTime.upgradedSkill.currTier;
	p90Upgrade.bulletSpread.upgradedSkill.maxTier = profile.p90.reloadTime.upgradedSkill.maxTier;
	p90Upgrade.bulletSpread.upgradedSkill.stat = profile.p90.reloadTime.upgradedSkill.stat;

	p90Upgrade.damage.upgradedSkill.currTier = profile.p90.damage.upgradedSkill.currTier;
	p90Upgrade.damage.upgradedSkill.maxTier = profile.p90.damage.upgradedSkill.maxTier;
	p90Upgrade.damage.upgradedSkill.stat = profile.p90.damage.upgradedSkill.stat;

	p90Upgrade.ammoCap.upgradedSkill.currTier = profile.p90.ammoCap.upgradedSkill.currTier;
	p90Upgrade.ammoCap.upgradedSkill.maxTier = profile.p90.ammoCap.upgradedSkill.maxTier;
	p90Upgrade.ammoCap.upgradedSkill.stat = profile.p90.ammoCap.upgradedSkill.stat;

	p90Upgrade.totalAmmo.upgradedSkill.currTier = profile.p90.totalAmmo.upgradedSkill.currTier;
	p90Upgrade.totalAmmo.upgradedSkill.maxTier = profile.p90.totalAmmo.upgradedSkill.maxTier;
	p90Upgrade.totalAmmo.upgradedSkill.stat = profile.p90.totalAmmo.upgradedSkill.stat;

	p90Upgrade.isBought = profile.p90.isBought;

#pragma endregion


#pragma region AK Loadin

	ak47Upgrade.magSize.upgradedSkill.currTier = profile.ak47.magSize.upgradedSkill.currTier;
	ak47Upgrade.magSize.upgradedSkill.maxTier = profile.ak47.magSize.upgradedSkill.maxTier;
	ak47Upgrade.magSize.upgradedSkill.stat = profile.ak47.magSize.upgradedSkill.stat;

	ak47Upgrade.reloadTime.upgradedSkill.currTier = profile.ak47.reloadTime.upgradedSkill.currTier;
	ak47Upgrade.reloadTime.upgradedSkill.maxTier = profile.ak47.reloadTime.upgradedSkill.maxTier;
	ak47Upgrade.reloadTime.upgradedSkill.stat = profile.ak47.reloadTime.upgradedSkill.stat;

	ak47Upgrade.recoilTime.upgradedSkill.currTier = profile.ak47.recoilTime.upgradedSkill.currTier;
	ak47Upgrade.recoilTime.upgradedSkill.maxTier = profile.ak47.recoilTime.upgradedSkill.maxTier;
	ak47Upgrade.recoilTime.upgradedSkill.stat = profile.ak47.recoilTime.upgradedSkill.stat;

	ak47Upgrade.bulletSpread.upgradedSkill.currTier = profile.ak47.bulletSpread.upgradedSkill.currTier;
	ak47Upgrade.bulletSpread.upgradedSkill.maxTier = profile.ak47.bulletSpread.upgradedSkill.maxTier;
	ak47Upgrade.bulletSpread.upgradedSkill.stat = profile.ak47.bulletSpread.upgradedSkill.stat;

	ak47Upgrade.damage.upgradedSkill.currTier = profile.ak47.damage.upgradedSkill.currTier;
	ak47Upgrade.damage.upgradedSkill.maxTier = profile.ak47.damage.upgradedSkill.maxTier;
	ak47Upgrade.damage.upgradedSkill.stat = profile.ak47.damage.upgradedSkill.stat;

	ak47Upgrade.ammoCap.upgradedSkill.currTier = profile.ak47.ammoCap.upgradedSkill.currTier;
	ak47Upgrade.ammoCap.upgradedSkill.maxTier = profile.ak47.ammoCap.upgradedSkill.maxTier;
	ak47Upgrade.ammoCap.upgradedSkill.stat = profile.ak47.ammoCap.upgradedSkill.stat;

	ak47Upgrade.totalAmmo.upgradedSkill.currTier = profile.ak47.totalAmmo.upgradedSkill.currTier;
	ak47Upgrade.totalAmmo.upgradedSkill.maxTier = profile.ak47.totalAmmo.upgradedSkill.maxTier;
	ak47Upgrade.totalAmmo.upgradedSkill.stat = profile.ak47.totalAmmo.upgradedSkill.stat;

	ak47Upgrade.isBought = profile.ak47.isBought;

#pragma endregion

#pragma region M16 Loadin

	m16Upgrade.magSize.upgradedSkill.currTier = profile.m16.magSize.upgradedSkill.currTier;
	m16Upgrade.magSize.upgradedSkill.maxTier = profile.m16.magSize.upgradedSkill.maxTier;
	m16Upgrade.magSize.upgradedSkill.stat = profile.m16.magSize.upgradedSkill.stat;

	m16Upgrade.reloadTime.upgradedSkill.currTier = profile.m16.reloadTime.upgradedSkill.currTier;
	m16Upgrade.reloadTime.upgradedSkill.maxTier = profile.m16.reloadTime.upgradedSkill.maxTier;
	m16Upgrade.reloadTime.upgradedSkill.stat = profile.m16.reloadTime.upgradedSkill.stat;

	m16Upgrade.recoilTime.upgradedSkill.currTier = profile.m16.recoilTime.upgradedSkill.currTier;
	m16Upgrade.recoilTime.upgradedSkill.maxTier = profile.m16.recoilTime.upgradedSkill.maxTier;
	m16Upgrade.recoilTime.upgradedSkill.stat = profile.m16.recoilTime.upgradedSkill.stat;

	m16Upgrade.bulletSpread.upgradedSkill.currTier = profile.m16.bulletSpread.upgradedSkill.currTier;
	m16Upgrade.bulletSpread.upgradedSkill.maxTier = profile.m16.bulletSpread.upgradedSkill.maxTier;
	m16Upgrade.bulletSpread.upgradedSkill.stat = profile.m16.bulletSpread.upgradedSkill.stat;

	m16Upgrade.damage.upgradedSkill.currTier = profile.m16.damage.upgradedSkill.currTier;
	m16Upgrade.damage.upgradedSkill.maxTier = profile.m16.damage.upgradedSkill.maxTier;
	m16Upgrade.damage.upgradedSkill.stat = profile.m16.damage.upgradedSkill.stat;

	m16Upgrade.ammoCap.upgradedSkill.currTier = profile.m16.ammoCap.upgradedSkill.currTier;
	m16Upgrade.ammoCap.upgradedSkill.maxTier = profile.m16.ammoCap.upgradedSkill.maxTier;
	m16Upgrade.ammoCap.upgradedSkill.stat = profile.m16.ammoCap.upgradedSkill.stat;

	m16Upgrade.totalAmmo.upgradedSkill.currTier = profile.m16.totalAmmo.upgradedSkill.currTier;
	m16Upgrade.totalAmmo.upgradedSkill.maxTier = profile.m16.totalAmmo.upgradedSkill.maxTier;
	m16Upgrade.totalAmmo.upgradedSkill.stat = profile.m16.totalAmmo.upgradedSkill.stat;

	m16Upgrade.isBought = profile.m16.isBought;

#pragma endregion


#pragma region LMG Loadin

	lmgUpgrade.magSize.upgradedSkill.currTier = profile.lmg.magSize.upgradedSkill.currTier;
	lmgUpgrade.magSize.upgradedSkill.maxTier = profile.lmg.magSize.upgradedSkill.maxTier;
	lmgUpgrade.magSize.upgradedSkill.stat = profile.lmg.magSize.upgradedSkill.stat;

	lmgUpgrade.reloadTime.upgradedSkill.currTier = profile.lmg.reloadTime.upgradedSkill.currTier;
	lmgUpgrade.reloadTime.upgradedSkill.maxTier = profile.lmg.reloadTime.upgradedSkill.maxTier;
	lmgUpgrade.reloadTime.upgradedSkill.stat = profile.lmg.reloadTime.upgradedSkill.stat;

	lmgUpgrade.recoilTime.upgradedSkill.currTier = profile.lmg.recoilTime.upgradedSkill.currTier;
	lmgUpgrade.recoilTime.upgradedSkill.maxTier = profile.lmg.recoilTime.upgradedSkill.maxTier;
	lmgUpgrade.recoilTime.upgradedSkill.stat = profile.lmg.recoilTime.upgradedSkill.stat;

	lmgUpgrade.bulletSpread.upgradedSkill.currTier = profile.lmg.bulletSpread.upgradedSkill.currTier;
	lmgUpgrade.bulletSpread.upgradedSkill.maxTier = profile.lmg.bulletSpread.upgradedSkill.maxTier;
	lmgUpgrade.bulletSpread.upgradedSkill.stat = profile.lmg.bulletSpread.upgradedSkill.stat;

	lmgUpgrade.damage.upgradedSkill.currTier = profile.lmg.damage.upgradedSkill.currTier;
	lmgUpgrade.damage.upgradedSkill.maxTier = profile.lmg.damage.upgradedSkill.maxTier;
	lmgUpgrade.damage.upgradedSkill.stat = profile.lmg.damage.upgradedSkill.stat;

	lmgUpgrade.ammoCap.upgradedSkill.currTier = profile.lmg.ammoCap.upgradedSkill.currTier;
	lmgUpgrade.ammoCap.upgradedSkill.maxTier = profile.lmg.ammoCap.upgradedSkill.maxTier;
	lmgUpgrade.ammoCap.upgradedSkill.stat = profile.lmg.ammoCap.upgradedSkill.stat;

	lmgUpgrade.totalAmmo.upgradedSkill.currTier = profile.lmg.totalAmmo.upgradedSkill.currTier;
	lmgUpgrade.totalAmmo.upgradedSkill.maxTier = profile.lmg.totalAmmo.upgradedSkill.maxTier;
	lmgUpgrade.totalAmmo.upgradedSkill.stat = profile.lmg.totalAmmo.upgradedSkill.stat;

	lmgUpgrade.isBought = profile.lmg.isBought;

#pragma endregion

#pragma region FlameThrower Loadin

	flameUpgrade.magSize.upgradedSkill.currTier = profile.flameThrower.magSize.upgradedSkill.currTier;
	flameUpgrade.magSize.upgradedSkill.maxTier = profile.flameThrower.magSize.upgradedSkill.maxTier;
	flameUpgrade.magSize.upgradedSkill.stat = profile.flameThrower.magSize.upgradedSkill.stat;

	flameUpgrade.reloadTime.upgradedSkill.currTier = profile.flameThrower.reloadTime.upgradedSkill.currTier;
	flameUpgrade.reloadTime.upgradedSkill.maxTier = profile.flameThrower.reloadTime.upgradedSkill.maxTier;
	flameUpgrade.reloadTime.upgradedSkill.stat = profile.flameThrower.reloadTime.upgradedSkill.stat;

	
	flameUpgrade.bulletSpread.upgradedSkill.currTier = profile.flameThrower.reloadTime.upgradedSkill.currTier;
	flameUpgrade.bulletSpread.upgradedSkill.maxTier = profile.flameThrower.reloadTime.upgradedSkill.maxTier;
	flameUpgrade.bulletSpread.upgradedSkill.stat = profile.flameThrower.reloadTime.upgradedSkill.stat;

	flameUpgrade.damage.upgradedSkill.currTier = profile.flameThrower.damage.upgradedSkill.currTier;
	flameUpgrade.damage.upgradedSkill.maxTier = profile.flameThrower.damage.upgradedSkill.maxTier;
	flameUpgrade.damage.upgradedSkill.stat = profile.flameThrower.damage.upgradedSkill.stat;

	flameUpgrade.bulletVelocity.upgradedSkill.currTier = profile.flameThrower.bulletVelocity.upgradedSkill.currTier;
	flameUpgrade.bulletVelocity.upgradedSkill.maxTier = profile.flameThrower.bulletVelocity.upgradedSkill.maxTier;
	flameUpgrade.bulletVelocity.upgradedSkill.stat = profile.flameThrower.bulletVelocity.upgradedSkill.stat;

	flameUpgrade.ammoCap.upgradedSkill.currTier = profile.flameThrower.ammoCap.upgradedSkill.currTier;
	flameUpgrade.ammoCap.upgradedSkill.maxTier = profile.flameThrower.ammoCap.upgradedSkill.maxTier;
	flameUpgrade.ammoCap.upgradedSkill.stat = profile.flameThrower.ammoCap.upgradedSkill.stat;

	flameUpgrade.totalAmmo.upgradedSkill.currTier = profile.flameThrower.totalAmmo.upgradedSkill.currTier;
	flameUpgrade.totalAmmo.upgradedSkill.maxTier = profile.flameThrower.totalAmmo.upgradedSkill.maxTier;
	flameUpgrade.totalAmmo.upgradedSkill.stat = profile.flameThrower.totalAmmo.upgradedSkill.stat;

	flameUpgrade.isBought = profile.flameThrower.isBought;


#pragma endregion

#pragma region Sniper Loadin

	sniperUpgrade.magSize.upgradedSkill.currTier = profile.sniper.magSize.upgradedSkill.currTier;
	sniperUpgrade.magSize.upgradedSkill.maxTier = profile.sniper.magSize.upgradedSkill.maxTier;
	sniperUpgrade.magSize.upgradedSkill.stat = profile.sniper.magSize.upgradedSkill.stat;

	sniperUpgrade.reloadTime.upgradedSkill.currTier = profile.sniper.reloadTime.upgradedSkill.currTier;
	sniperUpgrade.reloadTime.upgradedSkill.maxTier = profile.sniper.reloadTime.upgradedSkill.maxTier;
	sniperUpgrade.reloadTime.upgradedSkill.stat = profile.sniper.reloadTime.upgradedSkill.stat;
	
	sniperUpgrade.recoilTime.upgradedSkill.currTier = profile.sniper.recoilTime.upgradedSkill.currTier;
	sniperUpgrade.recoilTime.upgradedSkill.maxTier = profile.sniper.recoilTime.upgradedSkill.maxTier;
	sniperUpgrade.recoilTime.upgradedSkill.stat = profile.sniper.recoilTime.upgradedSkill.stat;


	sniperUpgrade.bulletSpread.upgradedSkill.currTier = profile.sniper.reloadTime.upgradedSkill.currTier;
	sniperUpgrade.bulletSpread.upgradedSkill.maxTier = profile.sniper.reloadTime.upgradedSkill.maxTier;
	sniperUpgrade.bulletSpread.upgradedSkill.stat = profile.sniper.reloadTime.upgradedSkill.stat;

	sniperUpgrade.damage.upgradedSkill.currTier = profile.sniper.damage.upgradedSkill.currTier;
	sniperUpgrade.damage.upgradedSkill.maxTier = profile.sniper.damage.upgradedSkill.maxTier;
	sniperUpgrade.damage.upgradedSkill.stat = profile.sniper.damage.upgradedSkill.stat;

	sniperUpgrade.ammoCap.upgradedSkill.currTier = profile.sniper.ammoCap.upgradedSkill.currTier;
	sniperUpgrade.ammoCap.upgradedSkill.maxTier = profile.sniper.ammoCap.upgradedSkill.maxTier;
	sniperUpgrade.ammoCap.upgradedSkill.stat = profile.sniper.ammoCap.upgradedSkill.stat;
	
	sniperUpgrade.penPower.upgradedSkill.currTier = profile.sniper.penPower.upgradedSkill.currTier;
	sniperUpgrade.penPower.upgradedSkill.maxTier = profile.sniper.penPower.upgradedSkill.maxTier;
	sniperUpgrade.penPower.upgradedSkill.stat = profile.sniper.penPower.upgradedSkill.stat;

	sniperUpgrade.totalAmmo.upgradedSkill.currTier = profile.sniper.totalAmmo.upgradedSkill.currTier;
	sniperUpgrade.totalAmmo.upgradedSkill.maxTier = profile.sniper.totalAmmo.upgradedSkill.maxTier;
	sniperUpgrade.totalAmmo.upgradedSkill.stat = profile.sniper.totalAmmo.upgradedSkill.stat;
	sniperUpgrade.isBought = profile.sniper.isBought;


#pragma endregion

#pragma region NadeLauncher Loadin

	nadeLauncherUpgrade.magSize.upgradedSkill.currTier = profile.nadeLauncher.magSize.upgradedSkill.currTier;
	nadeLauncherUpgrade.magSize.upgradedSkill.maxTier = profile.nadeLauncher.magSize.upgradedSkill.maxTier;
	nadeLauncherUpgrade.magSize.upgradedSkill.stat = profile.nadeLauncher.magSize.upgradedSkill.stat;

	nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier = profile.nadeLauncher.reloadTime.upgradedSkill.currTier;
	nadeLauncherUpgrade.reloadTime.upgradedSkill.maxTier = profile.nadeLauncher.reloadTime.upgradedSkill.maxTier;
	nadeLauncherUpgrade.reloadTime.upgradedSkill.stat = profile.nadeLauncher.reloadTime.upgradedSkill.stat;

	nadeLauncherUpgrade.damage.upgradedSkill.currTier = profile.nadeLauncher.damage.upgradedSkill.currTier;
	nadeLauncherUpgrade.damage.upgradedSkill.maxTier = profile.nadeLauncher.damage.upgradedSkill.maxTier;
	nadeLauncherUpgrade.damage.upgradedSkill.stat = profile.nadeLauncher.damage.upgradedSkill.stat;

	nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier = profile.nadeLauncher.bulletVelocity.upgradedSkill.currTier;
	nadeLauncherUpgrade.bulletVelocity.upgradedSkill.maxTier = profile.nadeLauncher.bulletVelocity.upgradedSkill.maxTier;
	nadeLauncherUpgrade.bulletVelocity.upgradedSkill.stat = profile.nadeLauncher.bulletVelocity.upgradedSkill.stat;

	nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier = profile.nadeLauncher.ammoCap.upgradedSkill.currTier;
	nadeLauncherUpgrade.ammoCap.upgradedSkill.maxTier = profile.nadeLauncher.ammoCap.upgradedSkill.maxTier;
	nadeLauncherUpgrade.ammoCap.upgradedSkill.stat = profile.nadeLauncher.ammoCap.upgradedSkill.stat;

	nadeLauncherUpgrade.totalAmmo.upgradedSkill.currTier = profile.nadeLauncher.totalAmmo.upgradedSkill.currTier;
	nadeLauncherUpgrade.totalAmmo.upgradedSkill.maxTier = profile.nadeLauncher.totalAmmo.upgradedSkill.maxTier;
	nadeLauncherUpgrade.totalAmmo.upgradedSkill.stat = profile.nadeLauncher.totalAmmo.upgradedSkill.stat;

	nadeLauncherUpgrade.isBought = profile.nadeLauncher.isBought;


#pragma endregion

}
