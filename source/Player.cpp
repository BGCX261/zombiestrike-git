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

#include "Zombie.h"
#include "Bullet.h"
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


	hud.Initialize(this);
	float rectoffset	= 9.0f;
	float rectheight	= 112.0f;
	m_rectAbilityPoint	= { 13.0f, Game::GetInstance()->GetScreenHeight() - 112.0f + 6.0f };
	m_rectAbilitySize	= { 95.0f, 102.0f };

	//pistol = new Pistol(this);
	//shotgun = new Shotgun(this);
	//arifle = new AssaultRifle(this);
	//sniper = new Sniper(this);
	//flameThrower = new FlameThrower(this);

	m_fCurrHP = m_fMaxHP = 100.0f;

	if (GameplayState::GetInstance()->GetGameMode() == true)
		profile = Game::GetInstance()->GetStoryProfile();
	else
		profile = Game::GetInstance()->GetSurvivalProfile();





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
	hud.Shutdown();

}

/*virtual*/ void Player::Update(float dt)	 /*override*/
{
	// controller
	if (controller != nullptr)
		controller->Update(dt, this, { 0, 0 });

	if (GameplayState::GetInstance()->GetGameMode() == true)
		Game::GetInstance()->GetStoryProfile().health = m_fCurrHP = profile.health;
	else
		Game::GetInstance()->GetSurvivalProfile().health = m_fCurrHP = profile.health;



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
	if (m_bIsAlive == false)
		return;


	switch (pOther->GetType())
	{
		case ObjectType::OBJ_SLOW_ZOMBIE:
		case ObjectType::OBJ_FAST_ZOMBIE:
		case ObjectType::OBJ_FAT_ZOMBIE:
		case ObjectType::OBJ_TANK_ZOMBIE:
		case ObjectType::OBJ_EXPLODING_ZOMBIE:
		{
			// get zombie
			const Zombie* zombie = dynamic_cast<const Zombie*>(pOther);
			
			// take damage
			profile.health -= zombie->GetDamage() * Game::GetInstance()->DeltaTime();

			// check death
			CheckDamage();
		}
		break;


		// vomit
		case ObjectType::OBJ_VOMIT:
		{
			// get vomit
			const Bullet* bullet = dynamic_cast<const Bullet*>(pOther);
			
			// take damage
			profile.health -= bullet->GetDamage() * Game::GetInstance()->DeltaTime();
			

			// check death
			CheckDamage();
		}
		break;


		// environment
		case OBJ_BARBEDWIRE:
		case OBJ_SANDBAG:
		case OBJ_WALL:
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
	if (m_nNumTurrets == 0)
		return;

	// create turret message
	CreateTurretMessage* pMsg = new CreateTurretMessage(this);
	pMsg->QueueMessage();
	pMsg = nullptr;

	m_nNumTurrets--;
}

void Player::CheckDamage(void)
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	GameplayState* pGameplay = GameplayState::GetInstance();


	// dead, !hurt
	if (profile.health <= 0.0f)
	{
		profile.health = 0.0f;

		if (m_hHurt != nullptr && pAudio->IsAudioPlaying(*m_hHurt) == true)
			pAudio->StopAudio(*m_hHurt);

		if (pAudio->IsAudioPlaying(*m_hDeath) == false)
			voice = pAudio->PlayAudio(*m_hDeath, false);
		pAudio->SetVoiceVolume(voice);

		m_bIsAlive = false;

		SetVelocity({ 0, 0 });
	}

	// hurt, !dead
	else
	{
		int sound = rand() % 3;

		switch (sound)
		{
		case 0:
			m_hHurt = &pGameplay->playerHurt1;
			break;
		case 1:
			m_hHurt = &pGameplay->playerHurt2;
			break;
		case 2:
			m_hHurt = &pGameplay->playerHurt3;
			break;
		default:
			break;
		}

		if (pAudio->IsAudioPlaying(pGameplay->playerHurt1) == false &&
			pAudio->IsAudioPlaying(pGameplay->playerHurt2) == false &&
			pAudio->IsAudioPlaying(pGameplay->playerHurt3) == false)
			voice = pAudio->PlayAudio(*m_hHurt, false);
		pAudio->SetVoiceVolume(voice);

		m_hHurt = nullptr;
	}
}

