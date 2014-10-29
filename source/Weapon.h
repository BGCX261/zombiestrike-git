#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Timer.h"

class Player;

class Weapon
{

protected:

	enum Type { PISTOL, SMG, SHOTGUN, ASSUALT_RIFLE, SPECIAL, NONE};
	enum Gun {GLOCK, REVOLVER, MAC10, TECH9, SP90 , SAWN, PUMP, AUTO, M16, LMG, AK47, GLAUNCHER, MINIGUN, FTHROWER};

	SGD::Rectangle m_rRenderRect;

	int type = NONE;
	int currAmmo = 0;
	int magSize = 0;
	int ammoCapactity = 0;
	int penetratingPower = 0;
	float recoilTime = 0.0f;
	float reloadTime = 0.0f;
	float damage = 0.0f;
	float speed = 0.0f;
	float lifeTime = 0.0f;
	bool m_bIsAutomatic = false;
	bool m_bIsObtained = false;
	Timer reloadTimer;
	Timer recoilTimer;
	Player* m_pOwner = nullptr;



public:
	Weapon();
	virtual ~Weapon();

	virtual void Fire(float dt);
	virtual void Update(float dt);

	//Accessors
	SGD::Rectangle GetRenderRect() { return m_rRenderRect; }
	int GetType(void) const { return type; }
	int GetMagSize(void) const { return magSize; }
	int GetAmmoCap(void) const { return ammoCapactity; }
	int GetPenetratingPower(void) const { return penetratingPower; }
	float GetRecoilTime(void) const { return recoilTime; }
	float GetReloadTime(void) const { return reloadTime; }
	float GetDamage(void) const { return damage; }
	float GetSpeed(void) const { return speed; }
	float GetLifeTime(void) const { return lifeTime; }
	bool GetObtained() { return m_bIsObtained; }
	bool GetAutomatic() { return m_bIsAutomatic; }
	Timer GetReloadTimer(void) const { return reloadTimer; }
	Timer GetRecoilTimer(void) const { return recoilTimer; }
	Player* GetOwner(void) const { return m_pOwner; }

	//Mutators

	void SetRenderRect(SGD::Rectangle rRect) { m_rRenderRect = rRect; }
	void SetType(int _type)  { type = _type; }
	void SetMagSize(int _magSize)  { magSize = _magSize; }
	void SetAmmoCap(int _ammoCap)  { ammoCapactity = _ammoCap; }
	void SetRecoilTime(float _recoilTime)  { recoilTime = _recoilTime; }
	void SetReloadTime(float _reloadTime)  { reloadTime = _reloadTime; }
	void SetDamage(float _damage)  { damage = _damage; }
	void SetSpeed(float _speed)  { speed = _speed; }
	void SetLifeTime(float _lifeTime)  { lifeTime = _lifeTime; }
	void SetOwner(Player* owner);
	void SetEquipped(bool isEquipped) { m_bIsAutomatic = isEquipped ; }
	void SetObtained(bool isObtained) { m_bIsObtained = isObtained; }

};

