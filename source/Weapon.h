#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Timer.h"

class MovingObject;

class Weapon
{

protected:

	enum Type { PISTOL, SHOTGUN, ASSUALT_RIFLE, SNIPER, FLAME_THROWER, NONE};

	int type = NONE;
	int currAmmo = 0;
	int magSize = 0;
	int totalAmmo = 0;
	int ammoCapactity = 0;
	int penetratingPower = 0;
	float bulletSpread = 0.0f;
	float recoilTime = 0.0f;
	float reloadTime = 0.0f;
	float damage = 0.0f;
	float speed = 0.0f;
	float lifeTime = 0.0f;
	Timer reloadTimer;
	Timer recoilTimer;
	MovingObject* m_pOwner = nullptr;
	



public:
	
	Weapon();

	virtual ~Weapon();

	virtual void Fire(float dt);
	virtual void Update(float dt);


	int GetType(void) const { return type; }
	int GetMagSize(void) const { return magSize; }
	int GetAmmoCap(void) const { return ammoCapactity; }
	int GetPenetratingPower(void) const { return penetratingPower; }
	float GetRecoilTime(void) const { return recoilTime; }
	float GetReloadTime(void) const { return reloadTime; }
	float GetDamage(void) const { return damage; }
	float GetSpeed(void) const { return speed; }
	float GetLifeTime(void) const { return lifeTime; }
	float GetBulletSpread(void) const { return bulletSpread; }
	int GetTotalAmmo(void) const { return totalAmmo; }
	Timer GetReloadTimer(void) const { return reloadTimer; }
	Timer GetRecoilTimer(void) const { return recoilTimer; }
	MovingObject* GetOwner(void) const { return m_pOwner; }

	void AddAmmo(int addedAmmo) { totalAmmo += addedAmmo; }
	void SetType(int _type)  { type = _type; }
	void SetMagSize(int _magSize)  { magSize = _magSize; }
	void SetAmmoCap(int _ammoCap)  { ammoCapactity = _ammoCap; }
	void SetRecoilTime(float _recoilTime)  { recoilTime = _recoilTime; }
	void SetReloadTime(float _reloadTime)  { reloadTime = _reloadTime; }
	void SetDamage(float _damage)  { damage = _damage; }
	void SetSpeed(float _speed)  { speed = _speed; }
	void SetLifeTime(float _lifeTime)  { lifeTime = _lifeTime; }
	void SetBulletSpread(float _bulletSpread) { bulletSpread = _bulletSpread; }
	void SetOwner(MovingObject* owner);

};

