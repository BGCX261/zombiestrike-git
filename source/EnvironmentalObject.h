#pragma once
#include "BaseObject.h"
#include "../SGD Wrappers/SGD_Listener.h"
class EnvironmentalObject : public BaseObject, public SGD::Listener
{
protected:

	int type = OBJ_BASE;
	std::string event;
	bool isActive = false;

public:
	EnvironmentalObject();
	virtual ~EnvironmentalObject();
	virtual void Render(void) {}
	virtual SGD::Rectangle GetRect(void) const;
	virtual int GetType(void) const { return type; }
	virtual void HandleCollision(const IBase* pOther) override;
	

	void SetType(int newType) { type = newType; }
	void SetEvent(std::string str_Event) { event = str_Event; }
	
};

