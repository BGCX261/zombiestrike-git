#pragma once
#include "IGameState.h"
#include "ShopStructures.h"
#include "GamerProfile.h"

#define BUTTON_WIDTH 123
#define BUTTON_HEIGHT 27


class ShopState : public IGameState
{
public:
	enum Pages { PISTOLS, SHOTGUNS, SMGS, ASSAULT_RIFLES, HEAVY, DEFENSE };

	/**********************************************************/
	// Singleton Accessor
	static ShopState* GetInstance(void);


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Input(void)				override;	// handle user input
	virtual void	Update(float elapsedTime)	override;	// update entites
	virtual void	Render(void)				override;	// render entities / menu

	void LoadShopStatus();


private:
	/**********************************************************/
	// SINGLETON!
	ShopState(void) = default;
	virtual ~ShopState(void) = default;

	ShopState(const ShopState&) = delete;
	ShopState& operator= (const ShopState&) = delete;

	PistolUpgrade pistolUpgrade;
	RevolverUpgrade revolverUpgrade;
	SMGUpgrade uziUpgrade;
	SMGUpgrade tech9Upgrade;
	SMGUpgrade p90Upgrade;
	AssualtRifleUpgrade ak47Upgrade;
	AssualtRifleUpgrade m16Upgrade;
	AssualtRifleUpgrade lmgUpgrade;
	SawnOffUpgrade sawnOffUpgrade;
	StdShotgunUpgrade pumpShotgunUpgrade;
	StdShotgunUpgrade autoShotgunUpgrade;
	SniperUpgrade sniperUpgrade;
	FlamethrowerUpgrade flameUpgrade;
	GrenadeLauncherUpgrade nadeLauncherUpgrade;

	SGD::Rectangle shotTab1 = { 350, 100, 350 + BUTTON_WIDTH, 127 };
	SGD::Rectangle shotTab2 = { shotTab1.right, 100, shotTab1.right + BUTTON_WIDTH, 127 };
	SGD::Rectangle shotTab3 = { shotTab2.right, 100, shotTab2.right + BUTTON_WIDTH, 127 };


	int currPage = 0;
	int currTab = 0;
	SGD::HTexture weaponsImage = SGD::INVALID_HANDLE;
	SGD::HTexture buyButton = SGD::INVALID_HANDLE;
	SGD::HTexture upgradeButton = SGD::INVALID_HANDLE;
	SGD::HTexture m_hReticleImage = SGD::INVALID_HANDLE;

	GamerProfile profile;
	SGD::Rectangle Buttons[9];
	SGD::Size screenSize;
};

