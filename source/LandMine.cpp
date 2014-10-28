#include "LandMine.h"
#include "AnimationManager.h"
#include "Animation.h"


LandMine::LandMine()
{
	this->SetType(ObjectType::OBJ_LANDMINE);
	this->SetAnimation("landmine");
}

LandMine::~LandMine()
{
}

void LandMine::Update( float dt )
{
	if (this->isActive == false)
		return;


	BaseObject::Update(dt);


	// on last frame of expolsion animation
	int numframes = AnimationManager::GetInstance()->GetAnimation("explode")->GetFrames().size();
	numframes--;

	if (this->GetAnimation() == "explode" && this->GetAnimationStamp().m_nCurrFrame == numframes)
	{
		// deactivate landmine
		this->isActive = false;
	}


}

void LandMine::Render( void )
{
	if (this->isActive == false)
		return;


	BaseObject::Render();
}

void LandMine::HandleCollision( const IBase* pOther )
{
	if (this->isActive == false)
		return;


	// zombie collision
	if (pOther->GetType() == ObjectType::OBJ_SLOW_ZOMBIE ||
		pOther->GetType() == ObjectType::OBJ_FAT_ZOMBIE || 
		pOther->GetType() == ObjectType::OBJ_FAST_ZOMBIE )
	{
		// activated mine
		if (this->GetAnimation() == "landmine")
		{
			// trigger explosion
			this->SetAnimation("explode");
		}

		// exploding mine
		else if (this->GetAnimation() == "explode")
		{
			// insta-kill zombies caught in explosion
		}
	}

}