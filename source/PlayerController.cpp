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

#include "WeaponManager.h"


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

	if (pInput->IsControllerConnected(0) == true)
	{
		if (pInput->GetRightJoystick(0).x != 0.0f && pInput->GetRightJoystick(0).y != 0.0f)
		{
			SGD::Vector toCursor = SGD::Point(pInput->GetRightJoystick(0).x * 1000 + GameplayState::GetInstance()->GetCamera()->GetPosition().x, pInput->GetRightJoystick(0).y * 1000 + GameplayState::GetInstance()->GetCamera()->GetPosition().y) - toUpdate->GetPosition();
			if (m_Player->m_bIsAlive == true)
			{
				toCursor.Normalize();
				if (toCursor.ComputeDotProduct(toUpdate->GetDirection()) < 0.999f)
				{
					if (toUpdate->GetDirection().ComputeSteering(toCursor) > 0)
						toUpdate->SetRotation(toUpdate->GetRotation() + (SGD::PI*2.0f  * dt)); //Turn left

					else if (toUpdate->GetDirection().ComputeSteering(toCursor) < 0)
						toUpdate->SetRotation(toUpdate->GetRotation() - (SGD::PI*2.0f  * dt)); //Turn right

					SGD::Vector orientation = { 0, -1 };
					orientation.Rotate(toUpdate->GetRotation());
					toUpdate->SetDirection(orientation);
				}
			}
		}
		
	}
	else
	{
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

	}
	


	std::string animation = "";
	animation = m_Player->m_bIsAlive == true ? m_Player->animation.m_strCurrAnimation : "playerDeath";


	// player is walking(playerWalk) OR running(playerRun)
	if ((pInput->IsKeyDown(SGD::Key::Shift) == true || pInput->IsButtonDown(0, 6) == true)  && m_Player->m_bIsAlive == true)
		m_Player->m_bIsSprinting = true;

	else if ((pInput->IsKeyDown(SGD::Key::Shift) == false || pInput->IsButtonDown(0, 6) == false) && m_Player->m_bIsAlive == true)
		m_Player->m_bIsSprinting = false;


	SGD::Vector newVelocity = { 0, 0 };

	// player moves forward/backward
	if ((pInput->IsKeyDown(SGD::Key::W) == true || pInput->GetLeftJoystick(0).y < 0) &&
		m_Player->m_bIsAlive == true)
	{
		newVelocity += {0, -1};
		m_Player->m_bMoving = true;
	}
	if (pInput->IsKeyDown(SGD::Key::S) || pInput->GetLeftJoystick(0).y > 0 && m_Player->m_bIsAlive == true)
	{
		newVelocity += {0, 1};
		m_Player->m_bMoving = true;
	}


	// player strafes left/right
	if (pInput->IsKeyDown(SGD::Key::A) == true || pInput->GetLeftJoystick(0).x < 0 && m_Player->m_bIsAlive == true)
	{
		newVelocity += {-1, 0};
		m_Player->m_bMoving = true;
	}
	if (pInput->IsKeyDown(SGD::Key::D) == true || pInput->GetLeftJoystick(0).x > 0 && m_Player->m_bIsAlive == true)
	{	
		newVelocity += {1, 0};
		m_Player->m_bMoving = true;
	}

	 if (pInput->IsControllerConnected(0) == true)
	 {
		 if (pInput->GetLeftJoystick(0).y == 0 && pInput->GetLeftJoystick(0).x == 0)
		 {
			 //animation = "playerIdle";
			 m_Player->m_bMoving = false;
		 } 
	 }
	 else
	 {
		 // player not moving
		 if ((pInput->IsKeyDown(SGD::Key::W) == false && pInput->IsKeyDown(SGD::Key::A) == false && pInput->IsKeyDown(SGD::Key::S) == false && pInput->IsKeyDown(SGD::Key::D) == false))

		 {
			 //animation = "playerIdle";
			 m_Player->m_bMoving = false;
		 }

	 }
	
	if (m_Player->IsSprinting() == true)
		m_Player->SetVelocity(newVelocity * m_Player->GetMoveSpeed() * 2);
	else
		m_Player->SetVelocity(newVelocity * m_Player->GetMoveSpeed());

	
	WeaponManager*		pWeaponManager = WeaponManager::GetInstance();

	// selecting abilities
	if (pInput->IsKeyPressed(SGD::Key::N1) == true)
		pWeaponManager->SelectWeapon(PISTOL);
	else if (pInput->IsKeyPressed(SGD::Key::N2) == true)
		pWeaponManager->SelectWeapon(SHOTGUN);

	else if(pInput->IsKeyPressed(SGD::Key::N3) == true)
		pWeaponManager->SelectWeapon(SMG);

	else if (pInput->IsKeyPressed(SGD::Key::N4) == true)
		pWeaponManager->SelectWeapon(ASSUALT_RIFLE);

	else if (pInput->IsKeyPressed(SGD::Key::N5) == true)
		pWeaponManager->SelectWeapon(SPECIAL);


	




	
		//m_Player->flameThrower->Fire(dt);
	if (m_Player->isPlacingTurret() == false)
	{
		if (WeaponManager::GetInstance()->GetSelected()->GetAutomatic())
		{
			if ((pInput->IsKeyDown(SGD::Key::MouseLeft) == true) || (pInput->IsButtonDown(0, 7) == true))
				pWeaponManager->GetSelected()->Fire(dt);


		}
		else
		{
			if ((pInput->IsKeyPressed(SGD::Key::MouseLeft) == true) || (pInput->IsButtonPressed(0, 7) == true))
				pWeaponManager->GetSelected()->Fire(dt);
		}


		if ((pInput->IsKeyPressed(SGD::Key::R) == true || (pInput->IsButtonPressed(0, 0) == true)) && pWeaponManager->GetSelected()->IsReloading() == false)
			pWeaponManager->GetSelected()->ReloadNeeded();

	}
	else
	{
		if ((pInput->IsKeyPressed(SGD::Key::MouseLeft) == true) || (pInput->IsButtonPressed(0, 1) == true))
			m_Player->SpawnTurret();

	}

		


	// spawning turrets
	if (pInput->IsKeyPressed(SGD::Key::T) == true || (pInput->IsButtonPressed(0, 3) == true))
		m_Player->SetIsPlacingTurret(!m_Player->isPlacingTurret());
	



	// not sure
	if (pInput->IsKeyPressed(SGD::Key::N) == true)
	{
		m_Player->hud.nofiticationBar.isActive = !m_Player->hud.nofiticationBar.isActive;
	}




	


	// player not doing anything (playerIdle)
	if (m_Player->m_bMoving == false)
	{
		m_Player->SetVelocity({ 0, 0 });
	//	animation = "playerIdle";
		m_Player->m_bMoving = false;
	}






	if (m_Player->GetAnimation() != animation)
	{
		m_Player->SetAnimation(animation);
	}
	


	return true;
	// nuh nuh
}
