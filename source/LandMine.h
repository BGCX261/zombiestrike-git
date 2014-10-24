#pragma once

#include "EnvironmentalObject.h"

class LandMine : public EnvironmentalObject
{
public:
	LandMine();
	~LandMine();

	void			Update			( float dt )			override;
	void			Render			( void )				override;
	void			HandleCollision	( const IBase* pOther )	override;


};

