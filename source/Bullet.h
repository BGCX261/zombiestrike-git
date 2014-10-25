#pragma once

#include "MovingObject.h"
#include "../SGD Wrappers/SGD_Handle.h"	
#include <string>

class MovingObject;

class Bullet : public MovingObject
{
public:
	Bullet() = default;
	virtual ~Bullet();

	virtual void		Update(float elapsedTime) override;
	virtual void		HandleCollision(const IBase* pOther)	override;

	virtual int			GetType(void) const override { return OBJ_BULLET; }
	float GetDamage(void) const { return damage; }
	float GetLifeTime(void) const { return lifeTime; }
	int GetPenPower(void) const { return penetratingPower; }

	void SetDamage(float _damage) { damage = _damage; }
	void SetLifeTime(float _lifeTime) { lifeTime = _lifeTime; }
	void SetPenPower(int _penPower) { penetratingPower = _penPower; }





	MovingObject*			GetOwner(void) const { return m_pOwner; }

	//Mutators
	void				SetOwner(MovingObject* _owner);
	bool				IsDead();

private:

	MovingObject* m_pOwner = nullptr;
	float damage = 0.0f;
	float lifeTime = 0.0f;
	int penetratingPower = 0;


	friend class BaseBehavior;


};

