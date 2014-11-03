#pragma once
#include "Weapon.h"
#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include <vector>

class Player;

using namespace std;

class WeaponManager : public Weapon
{   

	vector<Weapon*> m_vWeapons;

	int curIndex;
	
	//Image Assets
	SGD::HTexture * m_hHudWpn = nullptr;
	
	//Sound Assets
	SGD::HAudio * m_hWpnSwitch = nullptr;
	
public:

	WeaponManager() = default;
	virtual ~WeaponManager() = default;

	static WeaponManager* GetInstance(void);

	////Accessors////
	//Returns weapon vector
	vector<Weapon*> GetWeapons() { return m_vWeapons; }

	//Returns currently selected weapon
	Weapon * GetSelected() { return m_vWeapons[curIndex]; }

	SGD::HTexture * GetWeaponImage() { return m_hHudWpn; }

	////Mutators////
	//Sets currently selected weapon
	void SelectWeapon(int index);

	//Adds weapons to weapon vector
	void AddWeapons(Weapon*);

	void SetPistolImage(SGD::HTexture * hWpn) { m_hHudWpn = hWpn; }

	void Initialize(MovingObject& owner);
	void Render();
	void Input();
	void Update(float dt);
	void Exit();

	SGD::Rectangle SetImageRect(float width,  float height, unsigned int row, unsigned int col);

	//Weapon factory methods
	Weapon * CreateAssaultRifle();
	Weapon * CreatePistol();
	Weapon * CreateShotgun();
	Weapon * CreateSniper();
	Weapon * CreateFlameThrower();
	Weapon * CreateP90();

};

