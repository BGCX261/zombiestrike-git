#pragma once
#include "IGameState.h"
#include "ShopStructures.h"
#include "GamerProfile.h"
#include "Timer.h"
class ShopState : public IGameState{	
	
	ShopState() = default;
	virtual ~ShopState() = default;

	ShopState(const ShopState&) = delete;
	ShopState& operator=(const ShopState&) = delete;

	int m_nCursor = 0;

	//Shop time between waves
	Timer m_tShopTimer;

	bool m_bTimerSet = false;

public:
	enum Pages { PISTOLS, SHOTGUNS, SMGS, ASSAULT_RIFLES, HEAVY, DEFENSE };

	static ShopState* GetInstance(void);

	virtual void	Enter(void)				override;
	virtual void	Exit(void)				override;

	virtual bool	Input(void)				override;
	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;

	Timer GetShopTimer() const { return m_tShopTimer; }
	void LoadShopStatus();
private:
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

