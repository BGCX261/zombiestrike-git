#include "Player.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include "..\SGD Wrappers\SGD_InputManager.h"
#include "..\SGD Wrappers\SGD_Geometry.h"
#include "GameplayState.h"
#include "BaseBehavior.h"
#include "BehaviorManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Timer.h"
#include "Game.h"
#include <fstream>
#include "../SGD Wrappers/SGD_Handle.h"
#include "CreateTurretMessage.h"

#include "Pistol.h"
#include "Shotgun.h"
#include "AssaultRifle.h"
#include "Sniper.h"
#include "FlameThrower.h"
#include "EnvironmentalObject.h"



Player::Player() : Listener(this)
{
	m_Attributes.m_fCurrEnergy = 100.0f;
	m_Attributes.m_fMaxEnergy = 100.0f;
	//szSize = {68, 64};


	//RegisterForEvent("PICK_UP_CORE");
	//RegisterForEvent("PICK_UP_STIM");
	//RegisterForEvent("CHECKPOINT");
	//RegisterForEvent("LEVEL_COMPLETE");
	//RegisterForEvent("HIT");


	m_hDeath	= &GameplayState::GetInstance()->playerDeath;
	voice		= SGD::INVALID_HANDLE;


	//hud.Initialize();
	float rectoffset	= 9.0f;
	float rectheight	= 112.0f;
	m_rectAbilityPoint	= { 13.0f, Game::GetInstance()->GetScreenHeight() - 112.0f + 6.0f };
	m_rectAbilitySize	= { 95.0f, 102.0f };

	//pistol = new Pistol(this);
	//shotgun = new Shotgun(this);
	//arifle = new AssaultRifle(this);
	//sniper = new Sniper(this);
	//flameThrower = new FlameThrower(this);



}

Player::~Player()
{
	m_hDeath = nullptr;

	//UnregisterFromEvent("PICK_UP_CORE");
	//UnregisterFromEvent("PICK_UP_STIM");
	//UnregisterFromEvent("CHECKPOINT");
	//UnregisterFromEvent("LEVEL_COMPLETE");
	//UnregisterFromEvent("HIT");
	//delete pistol;
	//delete arifle;
	//delete shotgun;
	//delete sniper;
	//delete flameThrower;
	//hud.Shutdown();

}

/*virtual*/ void Player::Update(float dt)	 /*override*/
{
	// controller
	if (controller != nullptr)
		controller->Update(dt, this, { 0, 0 });


	//// camo
	//if (m_bIsCamoOn)
	//{
	//	// minus energy this frame
	//	m_Attributes.m_fCurrEnergy -= (m_Attributes.m_fCamoCost * m_Attributes.m_fCamoMultiplier) * dt;

	//	// cap energy at min
 //		if (m_Attributes.m_fCurrEnergy < 0.0f)
	//	{
	//		m_Attributes.m_fCurrEnergy = 0.0f;
	//		m_bIsCamoOn = false;
	//	}
	//}


	//// sprinting
	//if (m_bIsSprinting == true)
	//{
	//	// minus stamina this frame
	//	m_Attributes.m_fCurrStamina -= (m_Attributes.m_fSprintCost * dt);

	//	// cap stamina at min
	//	if (m_Attributes.m_fCurrStamina < 0.0f)
	//	{
	//		m_Attributes.m_fCurrStamina = 0.0f;
	//		m_bIsSprinting = false;
	//	}
	//}


	//// recover energy
	//if (m_Attributes.m_fCurrEnergy < m_Attributes.m_fMaxEnergy && m_bIsCamoOn == false)
	//{
	//	if (energyReboot.Update(dt))
	//		m_Attributes.m_fCurrEnergy += m_Attributes.m_fEnergyRegen * dt;

	//	if (m_Attributes.m_fCurrEnergy > m_Attributes.m_fMaxEnergy)
	//		m_Attributes.m_fCurrEnergy = m_Attributes.m_fMaxEnergy;
	//}


	//// recover stamina
	//if (m_Attributes.m_fCurrStamina < m_Attributes.m_fMaxStamina)
	//{
	//	if (m_bIsSprinting == false)
	//	{
	//		if (m_bMoving == false)
	//			m_Attributes.m_fCurrStamina += m_Attributes.m_fStaminaRegen * dt;
	//		else
	//			m_Attributes.m_fCurrStamina += (m_Attributes.m_fStaminaRegen *0.5f) * dt;
	//	}
	//}

	//pistol->Update(dt);
	//shotgun->Update(dt);
	//arifle->Update(dt);
	//sniper->Update(dt);
	//flameThrower->Update(dt);


	// hud skills rects
	switch (m_unCurrAbility)
	{
	case 0: // Camo
		m_rectAbilityPoint.x	= 13.0f;
		m_rectAbilitySize		= { 95.0f, 102.0f };
		break;

	case 1: //ppp
		m_rectAbilityPoint.x	= 116.0f;
		m_rectAbilitySize		= { 104.0f, 102.0f };
		break;

	case 2: // ppp
		m_rectAbilityPoint.x	= 226.0f;
		m_rectAbilitySize		= { 104.0f, 102.0f };
		break;

	case 3: // ppp
		m_rectAbilityPoint.x	= 336.0f;
		m_rectAbilitySize		= { 104.0f, 102.0f };
		break;

	default:
		break;
	}
	

	// update hud
	hud.Update(dt);


	// update movement
	MovingObject::Update(dt);
}

void Player::Render()
{
	// render player
	BaseObject::Render();


	// render hud
	hud.Render();


	// render hud selection rect
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(m_rectAbilityPoint, m_rectAbilitySize), { 0, 0, 0, 0 }, { 255, 0, 255, 0 });
}

/*virtual*/ void Player::HandleEvent(const SGD::Event* pEvent)
{
	/*if (pEvent->GetEventID() == "PICK_UP_CORE")
	{
		m_Attributes.m_fMaxEnergy += m_Attributes.m_fMaxEnergy * 0.10f;
		m_Attributes.m_fCurrEnergy = m_Attributes.m_fMaxEnergy;
	}
	else if (pEvent->GetEventID() == "PICK_UP_STIM")
	{
		m_Attributes.m_fMaxStamina += m_Attributes.m_fMaxStamina * 0.10f;
		m_Attributes.m_fCurrStamina = m_Attributes.m_fMaxStamina;
	}
	else if (pEvent->GetEventID() == "HIT")
	{
		m_bIsAlive = false;
	}
	else if (pEvent->GetEventID() == "CHECKPOINT")
	{
		profile.CamoMultiplier		= m_Attributes.m_fCamoMultiplier;					
		profile.CheckPointReached	= true;
		profile.MaxEnergy			= m_Attributes.m_fMaxEnergy;
		profile.MaxStamina			= m_Attributes.m_fCurrStamina;
		profile.RadarMultiplier		= m_Attributes.m_fRadarMultiplier;
		profile.SpeedMultiplier		= m_Attributes.m_fSpeedMultiplier;

		fstream file;

		file.open(profile.path.c_str());
		
		if (file.is_open())
		{
			file << profile.path << '\n';
			file << profile.MaxEnergy << '\n';
			file << profile.MaxStamina << '\n';
			file << profile.CamoMultiplier << '\n';
			file << profile.SpeedMultiplier << '\n';
			file << profile.RadarMultiplier << '\n';
			file << profile.LevelsComplete << '\n';
			file << profile.m_bHasKey << '\n';
			file << profile.CheckPointReached << '\n';
			file.close();
		}
	}
	else if (pEvent->GetEventID() == "LEVEL_COMPLETE")
	{
		profile.CamoMultiplier		= m_Attributes.m_fCamoMultiplier;
		profile.CheckPointReached	= true;
		profile.MaxEnergy			= m_Attributes.m_fMaxEnergy;
		profile.MaxStamina			= m_Attributes.m_fCurrStamina;
		profile.RadarMultiplier		= m_Attributes.m_fRadarMultiplier;
		profile.SpeedMultiplier		= m_Attributes.m_fSpeedMultiplier;

		Game::GetInstance()->GetProfile() = profile;
		fstream file;

		file.open(profile.path.c_str());

		if (file.is_open())
		{
			file << profile.path << '\n';
			file << profile.MaxEnergy << '\n';
			file << profile.MaxStamina << '\n';
			file << profile.CamoMultiplier << '\n';
			file << profile.SpeedMultiplier << '\n';
			file << profile.RadarMultiplier << '\n';
			file << (profile.LevelsComplete+1) << '\n';
			file << profile.m_bHasKey << '\n';
			file << profile.CheckPointReached << '\n';
			file.close();
		}

		m_bLevelCompleted = true;
	}*/
}

/*virtual*/ void Player::HandleCollision(const IBase* pOther) /*override*/
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	switch (pOther->GetType())
	{
		case ObjectType::OBJ_SLOW_ZOMBIE:
		{
			//if (pAudio->IsAudioPlaying(*m_hDeath) == false && m_bIsAlive == true)
			//	voice = pAudio->PlayAudio(*m_hDeath, false);
			//pAudio->SetVoiceVolume(voice);

			//m_bIsAlive = false;
		}
		break;

		case OBJ_BARBEDWIRE:
		case OBJ_SANDBAG:
		{
			const EnvironmentalObject* temp = dynamic_cast<const EnvironmentalObject*>(pOther);
			if (temp->IsActive())
				MovingObject::HandleCollision(pOther);
		}
		break;




	}
}

void Player::RetrieveBehavior(std::string name)
{

	if (controller != nullptr)
	{
		controller = nullptr;
	}
	controller = BehaviorManager::GetInstance()->GetBehaviors()[name];
}

/*virtual*/ void Player::Attack(void)
{


}

void Player::SpawnTurret(void)
{
	if (m_nNumTurrets == 3)
		return;

	// create turret message
	CreateTurretMessage* pMsg = new CreateTurretMessage(this);
	pMsg->QueueMessage();
	pMsg = nullptr;

	m_nNumTurrets++;
}

