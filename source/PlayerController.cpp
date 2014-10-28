#include "PlayerController.h"
#include "MovingObject.h"
#include "Weapon.h"
#include "..\SGD Wrappers\SGD_InputManager.h"
#include "DestroyObjectMessage.h"
#include "Player.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

/*virtual*/ bool PlayerController::Update(float dt, MovingObject* toUpdate, SGD::Point wayPoint)
{
	Player* m_Player = dynamic_cast<Player*>(toUpdate);

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// rotate to face mouse
	SGD::Vector toMouse = SGD::Point(pInput->GetMousePosition().x + GameplayState::GetInstance()->GetCamera()->GetPosition().x, pInput->GetMousePosition().y + GameplayState::GetInstance()->GetCamera()->GetPosition().y) - toUpdate->GetPosition();
	if (m_Player->m_bIsAlive == true)
	{
		toMouse.Normalize();
		if (toMouse.ComputeDotProduct(toUpdate->GetDirection()) < 0.999f)
		{
			if (toUpdate->GetDirection().ComputeSteering(toMouse) > 0)
				toUpdate->SetRotation(toUpdate->GetRotation() + (SGD::PI*2.0f  * dt)); //Turn left

			else if (toUpdate->GetDirection().ComputeSteering(toMouse) < 0)
				toUpdate->SetRotation(toUpdate->GetRotation() - (SGD::PI*2.0f  * dt)); //Turn right

			SGD::Vector orientation = { 0, -1 };
			orientation.Rotate(toUpdate->GetRotation());
			toUpdate->SetDirection(orientation);
		}
	}
	


//	std::string animation = "";
	//animation = m_Player->m_bIsAlive == true ? m_Player->animation.m_strCurrAnimation : "playerDeath";


	// player is walking(playerWalk) OR running(playerRun)
	if ((pInput->IsKeyDown(SGD::Key::Shift) == true || pInput->IsButtonDown(0, 6) == true) && m_Player->m_bMoving == true && m_Player->m_bIsAlive == true)
	{
		if (m_Player->m_bIsCamoOn == false)
		{
			if (m_Player->m_Attributes.m_fCurrStamina > 0)
			{
				m_Player->m_bIsSprinting = true;
				//animation = "playerRun";
			}
			else
			{
				m_Player->m_bIsSprinting = false;
			//	animation = "playerWalk";
			}

		}
	} 
	else if ((pInput->IsKeyDown(SGD::Key::Shift) == false || pInput->IsButtonDown(0, 6) == false) && m_Player->m_bMoving == true && m_Player->m_bIsAlive == true)
	{
		m_Player->m_bIsSprinting = false;
	//	animation = "playerWalk";
	} 



	// player moves forward/backward
	if ((pInput->IsKeyDown(SGD::Key::W) == true || pInput->GetLeftJoystick(0).y < 0) &&
		m_Player->m_bIsAlive == true)
	{
		if (m_Player->m_bIsSprinting == true)
			m_Player->SetVelocity(toMouse * m_Player->GetMoveSpeed() * 2);
		else
			m_Player->SetVelocity(toMouse* m_Player->GetMoveSpeed());

		m_Player->m_bMoving = true;
	}
	else if (pInput->IsKeyDown(SGD::Key::S) == true && m_Player->m_bIsAlive == true)
	{
		if (m_Player->m_bIsSprinting == true)
			m_Player->SetVelocity(-toMouse * m_Player->GetMoveSpeed() * 2);
		else
			m_Player->SetVelocity(-toMouse * m_Player->GetMoveSpeed());

		m_Player->m_bMoving = true;
	}


	// player strafes left/right
	if (pInput->IsKeyDown(SGD::Key::A) == true && m_Player->m_bIsAlive == true)
	{
		SGD::Vector strafeLeftVect = m_Player->GetDirection();
		strafeLeftVect.Rotate(SGD::PI*0.5f);

		if (m_Player->m_bIsSprinting == true)
			m_Player->SetVelocity(-strafeLeftVect * m_Player->GetMoveSpeed() * 2);
		else
			m_Player->SetVelocity(-strafeLeftVect * m_Player->GetMoveSpeed());

		m_Player->m_bMoving = true;
	}
	else if (pInput->IsKeyDown(SGD::Key::D) == true && m_Player->m_bIsAlive == true)
	{
		SGD::Vector strafeLeftVect = m_Player->GetDirection();
		strafeLeftVect.Rotate(SGD::PI*0.5f);

		if (m_Player->m_bIsSprinting == true)
			m_Player->SetVelocity(strafeLeftVect * m_Player->GetMoveSpeed() * 2);
		else
			m_Player->SetVelocity(strafeLeftVect * m_Player->GetMoveSpeed());

		m_Player->m_bMoving = true;
	}


	// player not moving
	if (pInput->IsKeyDown(SGD::Key::W) == false && pInput->IsKeyDown(SGD::Key::A) == false && pInput->IsKeyDown(SGD::Key::S) == false && pInput->IsKeyDown(SGD::Key::D) == false)
	{
		//animation = "playerIdle";
		m_Player->m_bMoving = false;
	}

	// selecting abilities
	if (pInput->IsKeyPressed(SGD::Key::N1) == true)
		m_Player->m_unCurrAbility = 0;
	else if (pInput->IsKeyPressed(SGD::Key::N2) == true)
		m_Player->m_unCurrAbility = 1;
	else if(pInput->IsKeyPressed(SGD::Key::N3) == true)
		m_Player->m_unCurrAbility = 2;
	else if (pInput->IsKeyPressed(SGD::Key::N4) == true)
		m_Player->m_unCurrAbility = 3;


	if ((pInput->IsKeyPressed(SGD::Key::MouseLeft) == true))
	{
		m_Player->flameThrower->Fire(dt);
	}

	// player activates an ability
	if ((pInput->IsKeyPressed(SGD::Key::Space) == true || pInput->IsButtonPressed(0, 0) == true) && m_Player->m_bIsAlive == true)
	{
		switch (m_Player->GetCurrAbility())
		{
		case 0: // Camo
			if ( m_Player->m_bIsCamoOn == false)
			{
				if (m_Player->m_Attributes.m_fCurrEnergy > 0.0f)
				{
 					m_Player->m_bIsCamoOn = true;
					m_Player->m_bIsSprinting = false; 
					m_Player->SetAlpha(125);
				}
				else
				{
					SGD::HAudio cannotuse = GameplayState::GetInstance()->cannot_use_skill;

					if (pAudio->IsAudioPlaying(cannotuse) == false)
						m_Player->SetVoice(pAudio->PlayAudio(cannotuse, false));

					pAudio->SetVoiceVolume(m_Player->GetVoice());
				}
			}
			else
			{
				m_Player->m_bIsCamoOn = false;
				m_Player->SetAlpha(255);

			}
			break;

		case 1: // ppp
			break;

		case 2: // ppp
			break;

		case 3: // ppp
			break;

		default:
			break;
		}
	}


	// stun
	if (pInput->IsKeyPressed(SGD::Key::M) == true)
	{
		SGD::Event* msg = new SGD::Event("STUNNED");
		msg->QueueEvent();
		msg = nullptr;
	}


	// not sure
	if (pInput->IsKeyPressed(SGD::Key::N) == true)
	{
		m_Player->hud.nofiticationBar.isActive = !m_Player->hud.nofiticationBar.isActive;
	}




	// player suicide
	if (pInput->IsKeyPressed(SGD::Key::Eight) == true && m_Player->m_bIsAlive == true)
	{
		SGD::HAudio * death = m_Player->GetDeathSFX();

		if (pAudio->IsAudioPlaying(*death) == false)
			m_Player->SetVoice(pAudio->PlayAudio(*death, false)); //voice = pAudio->PlayAudio(*alarmSound, true);
		pAudio->SetVoiceVolume(m_Player->GetVoice());

	//	animation = "playerDeath";
		m_Player->SetVelocity({ 0, 0 });
		m_Player->m_bMoving = false;
		m_Player->m_bIsAlive = false;
	}


	// player not doing anything (playerIdle)
	if ((pInput->IsAnyKeyDown() == false || m_Player->m_bMoving == false) && m_Player->m_bIsAlive == true)
	{
		m_Player->SetVelocity({ 0, 0 });
	//	animation = "playerIdle";
		m_Player->m_bMoving = false;
	}

	else if ((pInput->IsAnyKeyDown() == false || m_Player->m_bMoving == false) && m_Player->m_bIsAlive == false)
	{
		m_Player->SetVelocity({ 0, 0 });
	//	animation = "playerDeath";
		m_Player->m_bMoving = false;
	}




	//if (m_Player->GetAnimation() != animation)
	//{
	//	m_Player->SetAnimation(animation);
	//}
	//


	return true;
	// nuh nuh
}
