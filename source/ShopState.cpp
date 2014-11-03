#include "ShopState.h"
#include "WeaponManager.h"
#include "Weapon.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"




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
	screenSize = SGD::Size( Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() );

	weaponsImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/weapons.png");
	buyButton = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/rectangle1.png");
	upgradeButton = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/rectangle2.png");


	LoadShopStatus();
	float startY = screenSize.height * .4f;

	for (unsigned int currWeapon = 0; currWeapon < 8; currWeapon++)
	{
		Buttons[currWeapon] = SGD::Rectangle(SGD::Point( screenSize.width * 0.5f, (float)startY ), SGD::Size(BUTTON_WIDTH, BUTTON_HEIGHT));
		startY += BUTTON_HEIGHT + 5;
	}
	Buttons[8] = SGD::Rectangle({ screenSize.width* 0.7f, screenSize.height * 0.7f }, SGD::Size(BUTTON_WIDTH, BUTTON_HEIGHT));

	
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

	switch (currPage)
	{
	case PISTOLS:
		if (pInput->IsKeyPressed(SGD::Key::Tab) == true)
		{
			currTab++;
			if (currTab > 1)
				currTab = 0;
		}
		break;
	case SHOTGUNS:
	case SMGS:
	case ASSAULT_RIFLES:
	case HEAVY:
		if (pInput->IsKeyPressed(SGD::Key::Tab) == true)
		{
			currTab++;
			if (currTab > 2)
				currTab = 0;
		}
		break;
	}

	
	return true;
}
void	ShopState::Update(float elapsedTime)
{

}
void	ShopState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	WeaponManager* pWeaponManager = WeaponManager::GetInstance(); 

	



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
						
						if (pistolUpgrade.magSize.upgradedSkill.currTier < pistolUpgrade.magSize.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (pistolUpgrade.magSize.upgradedSkill.currTier < pistolUpgrade.reloadTime.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });
						
						if (pistolUpgrade.magSize.upgradedSkill.currTier < pistolUpgrade.recoilTime.upgradedSkill.maxTier)
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
					
						if (revolverUpgrade.magSize.upgradedSkill.currTier < revolverUpgrade.magSize.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.reloadTime.upgradedSkill.currTier < revolverUpgrade.reloadTime.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.recoilTime.upgradedSkill.currTier < revolverUpgrade.recoilTime.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.penPower.upgradedSkill.currTier < revolverUpgrade.penPower.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.damage.upgradedSkill.currTier < revolverUpgrade.damage.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.ammoCap.upgradedSkill.currTier < revolverUpgrade.ammoCap.upgradedSkill.maxTier)
							pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						if (revolverUpgrade.totalAmmo.upgradedSkill.currTier < revolverUpgrade.totalAmmo.upgradedSkill.maxTier)
							pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						else
							pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					}
					
					pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });

				


	}

		break;
	case SHOTGUNS:
	{
					 pGraphics->DrawTexture(upgradeButton, { shotTab1.left, shotTab1.top }, {}, {}, {}, { 0.5f, 0.5f });
					 pGraphics->DrawTexture(upgradeButton, { shotTab2.left, shotTab2.top }, {}, {}, {}, { 0.5f, 0.5f });
					 pGraphics->DrawTexture(upgradeButton, { shotTab3.left, shotTab3.top }, {}, {}, {}, { 0.5f, 0.5f });
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
					
							  

							   if (sawnOffUpgrade.recoilTime.upgradedSkill.currTier < sawnOffUpgrade.recoilTime.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.reloadTime.upgradedSkill.currTier < sawnOffUpgrade.reloadTime.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });
							  
							   if (sawnOffUpgrade.bulletSpread.upgradedSkill.currTier < sawnOffUpgrade.bulletSpread.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });
							 
							   if (sawnOffUpgrade.damage.upgradedSkill.currTier < sawnOffUpgrade.damage.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.ammoCap.upgradedSkill.currTier < sawnOffUpgrade.ammoCap.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (sawnOffUpgrade.totalAmmo.upgradedSkill.currTier < sawnOffUpgrade.totalAmmo.upgradedSkill.maxTier)
								   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 
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
							  

							   if (pumpShotgunUpgrade.magSize.upgradedSkill.currTier < pumpShotgunUpgrade.magSize.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });
 
							   if (pumpShotgunUpgrade.recoilTime.upgradedSkill.currTier < pumpShotgunUpgrade.recoilTime.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.reloadTime.upgradedSkill.currTier < pumpShotgunUpgrade.reloadTime.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.bulletSpread.upgradedSkill.currTier < pumpShotgunUpgrade.bulletSpread.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.damage.upgradedSkill.currTier < pumpShotgunUpgrade.damage.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.ammoCap.upgradedSkill.currTier < pumpShotgunUpgrade.ammoCap.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (pumpShotgunUpgrade.totalAmmo.upgradedSkill.currTier < pumpShotgunUpgrade.totalAmmo.upgradedSkill.maxTier)
								   pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

					 
					 }
						 break;
					 case 2:
					 {
							   string autoMagSizeLevel = "LeveL ";
							   autoMagSizeLevel += std::to_string(pumpShotgunUpgrade.magSize.upgradedSkill.currTier);
							   autoMagSizeLevel += " ";

							   string autoRecoilLevel = "LeveL ";
							   autoRecoilLevel += std::to_string(pumpShotgunUpgrade.recoilTime.upgradedSkill.currTier);
							   autoRecoilLevel += " ";

							   string autoReloadLevel = "LeveL ";
							   autoReloadLevel += std::to_string(pumpShotgunUpgrade.reloadTime.upgradedSkill.currTier);
							   autoReloadLevel += " ";

							   string autoBullSpreadLevel = "LeveL ";
							   autoBullSpreadLevel += std::to_string(pumpShotgunUpgrade.bulletSpread.upgradedSkill.currTier);
							   autoBullSpreadLevel += " ";

							   string autoAmmoCapLevel = "LeveL ";
							   autoAmmoCapLevel += std::to_string(pumpShotgunUpgrade.ammoCap.upgradedSkill.currTier);
							   autoAmmoCapLevel += " ";

							   string autoDamageLevel = "LeveL ";
							   autoDamageLevel += std::to_string(pumpShotgunUpgrade.damage.upgradedSkill.currTier);
							   autoDamageLevel += " ";

							   pFont->Draw("Pump Action Shotgun: ", { screenSize.width *.1f, screenSize.height * 0.3f }, 1.0f, { 255, 255, 0, 0 });
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
					
							   if (autoShotgunUpgrade.magSize.upgradedSkill.currTier < autoShotgunUpgrade.magSize.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.recoilTime.upgradedSkill.currTier < autoShotgunUpgrade.recoilTime.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.reloadTime.upgradedSkill.currTier < autoShotgunUpgrade.reloadTime.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.bulletSpread.upgradedSkill.currTier < autoShotgunUpgrade.bulletSpread.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.damage.upgradedSkill.currTier < autoShotgunUpgrade.damage.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.ammoCap.upgradedSkill.currTier < autoShotgunUpgrade.ammoCap.upgradedSkill.maxTier)
								   pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							   if (autoShotgunUpgrade.totalAmmo.upgradedSkill.currTier < autoShotgunUpgrade.totalAmmo.upgradedSkill.maxTier)
								   pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							   else
								   pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });
					 
					 }
						 break;

					 }
					
					
					 pGraphics->DrawTexture(upgradeButton, { Buttons[8].left, Buttons[8].top }, {}, {}, {}, { 0.5f, 0.5f });

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



						   if (uziUpgrade.magSize.upgradedSkill.currTier < uziUpgrade.magSize.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.reloadTime.upgradedSkill.currTier < uziUpgrade.reloadTime.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.bulletSpread.upgradedSkill.currTier < uziUpgrade.bulletSpread.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.damage.upgradedSkill.currTier < uziUpgrade.damage.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.ammoCap.upgradedSkill.currTier < uziUpgrade.ammoCap.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (uziUpgrade.totalAmmo.upgradedSkill.currTier < uziUpgrade.totalAmmo.upgradedSkill.maxTier)
							   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						  


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


						   if (tech9Upgrade.magSize.upgradedSkill.currTier < tech9Upgrade.magSize.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.reloadTime.upgradedSkill.currTier < tech9Upgrade.reloadTime.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.bulletSpread.upgradedSkill.currTier < tech9Upgrade.bulletSpread.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.damage.upgradedSkill.currTier < tech9Upgrade.damage.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.ammoCap.upgradedSkill.currTier < tech9Upgrade.ammoCap.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (tech9Upgrade.totalAmmo.upgradedSkill.currTier < tech9Upgrade.totalAmmo.upgradedSkill.maxTier)
							   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

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


						   if (p90Upgrade.magSize.upgradedSkill.currTier < p90Upgrade.magSize.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.reloadTime.upgradedSkill.currTier < p90Upgrade.reloadTime.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.bulletSpread.upgradedSkill.currTier < p90Upgrade.bulletSpread.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.damage.upgradedSkill.currTier < p90Upgrade.damage.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.ammoCap.upgradedSkill.currTier < uziUpgrade.ammoCap.upgradedSkill.maxTier)
							   pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

						   if (p90Upgrade.totalAmmo.upgradedSkill.currTier < p90Upgrade.totalAmmo.upgradedSkill.maxTier)
							   pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
						   else
							   pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

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


									 if (ak47Upgrade.magSize.upgradedSkill.currTier < ak47Upgrade.magSize.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.reloadTime.upgradedSkill.currTier < ak47Upgrade.reloadTime.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.recoilTime.upgradedSkill.currTier < ak47Upgrade.recoilTime.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.bulletSpread.upgradedSkill.currTier < ak47Upgrade.bulletSpread.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.damage.upgradedSkill.currTier < ak47Upgrade.damage.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.ammoCap.upgradedSkill.currTier < ak47Upgrade.ammoCap.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (ak47Upgrade.totalAmmo.upgradedSkill.currTier < ak47Upgrade.totalAmmo.upgradedSkill.maxTier)
										 pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

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


									 if (m16Upgrade.magSize.upgradedSkill.currTier < m16Upgrade.magSize.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.reloadTime.upgradedSkill.currTier < m16Upgrade.reloadTime.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.recoilTime.upgradedSkill.currTier < m16Upgrade.recoilTime.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.bulletSpread.upgradedSkill.currTier < m16Upgrade.bulletSpread.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.damage.upgradedSkill.currTier < m16Upgrade.damage.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.ammoCap.upgradedSkill.currTier < m16Upgrade.ammoCap.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (m16Upgrade.totalAmmo.upgradedSkill.currTier < m16Upgrade.totalAmmo.upgradedSkill.maxTier)
										 pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });


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


									 if (lmgUpgrade.magSize.upgradedSkill.currTier < lmgUpgrade.magSize.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.reloadTime.upgradedSkill.currTier < lmgUpgrade.reloadTime.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.recoilTime.upgradedSkill.currTier < lmgUpgrade.recoilTime.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.bulletSpread.upgradedSkill.currTier < lmgUpgrade.bulletSpread.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.damage.upgradedSkill.currTier < lmgUpgrade.damage.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.ammoCap.upgradedSkill.currTier < lmgUpgrade.ammoCap.upgradedSkill.maxTier)
										 pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

									 if (lmgUpgrade.totalAmmo.upgradedSkill.currTier < lmgUpgrade.totalAmmo.upgradedSkill.maxTier)
										 pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
									 else
										 pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

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

							
							if (sniperUpgrade.magSize.upgradedSkill.currTier < sniperUpgrade.magSize.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.reloadTime.upgradedSkill.currTier < sniperUpgrade.reloadTime.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.recoilTime.upgradedSkill.currTier < sniperUpgrade.recoilTime.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.bulletSpread.upgradedSkill.currTier < sniperUpgrade.bulletSpread.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.damage.upgradedSkill.currTier < sniperUpgrade.damage.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.penPower.upgradedSkill.currTier < sniperUpgrade.recoilTime.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.ammoCap.upgradedSkill.currTier < sniperUpgrade.ammoCap.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[6].left + 5, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[6].left + 25, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (sniperUpgrade.totalAmmo.upgradedSkill.currTier < sniperUpgrade.totalAmmo.upgradedSkill.maxTier)
								pFont->Draw("Buy Ammo", { Buttons[7].left + 18, Buttons[7].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Ammo Full", { Buttons[7].left + 18, Buttons[7].top + 5 }, 0.5f, { 255, 255, 0, 0 });
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
							
							if (flameUpgrade.magSize.upgradedSkill.currTier < flameUpgrade.magSize.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.reloadTime.upgradedSkill.currTier < flameUpgrade.reloadTime.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							
							if (flameUpgrade.bulletSpread.upgradedSkill.currTier < flameUpgrade.bulletSpread.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.damage.upgradedSkill.currTier < flameUpgrade.damage.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.bulletVelocity.upgradedSkill.currTier < flameUpgrade.bulletVelocity.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							
							if (flameUpgrade.ammoCap.upgradedSkill.currTier < flameUpgrade.ammoCap.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[5].left + 5, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[5].left + 25, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (flameUpgrade.totalAmmo.upgradedSkill.currTier < flameUpgrade.totalAmmo.upgradedSkill.maxTier)
								pFont->Draw("Buy Ammo", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Ammo Full", { Buttons[6].left + 18, Buttons[6].top + 5 }, 0.5f, { 255, 255, 0, 0 });
							
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

							if (nadeLauncherUpgrade.magSize.upgradedSkill.currTier < nadeLauncherUpgrade.magSize.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[0].left + 5, Buttons[0].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[0].left + 25, Buttons[0].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.reloadTime.upgradedSkill.currTier < nadeLauncherUpgrade.reloadTime.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[1].left + 5, Buttons[1].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[1].left + 25, Buttons[1].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.damage.upgradedSkill.currTier < nadeLauncherUpgrade.damage.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[2].left + 5, Buttons[2].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[2].left + 25, Buttons[2].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.bulletVelocity.upgradedSkill.currTier < nadeLauncherUpgrade.bulletVelocity.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[3].left + 5, Buttons[3].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[3].left + 25, Buttons[3].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.ammoCap.upgradedSkill.currTier < nadeLauncherUpgrade.ammoCap.upgradedSkill.maxTier)
								pFont->Draw("Upgrade", { Buttons[4].left + 5, Buttons[4].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Maxed", { Buttons[4].left + 25, Buttons[4].top + 5 }, 0.5f, { 255, 255, 0, 0 });

							if (nadeLauncherUpgrade.totalAmmo.upgradedSkill.currTier < nadeLauncherUpgrade.totalAmmo.upgradedSkill.maxTier)
								pFont->Draw("Buy Ammo", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 0, 0, 255 });
							else
								pFont->Draw("Ammo Full", { Buttons[5].left + 18, Buttons[5].top + 5 }, 0.5f, { 255, 255, 0, 0 });

				  }
					  break;
				  }
				 

	}
		break;
	case DEFENSE:
	{

	}
		break;

	}

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

#pragma endregion
}
