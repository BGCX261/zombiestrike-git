#include "HTPGameState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "WinGameState.h"
#include "LoseGameState.h"
#include "ShopState.h"
#include "GameplayState.h"
#include "IntroState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_EventManager.h" 
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "DestroyObjectMessage.h"
#include "CreatePistolBullet.h"
#include "CreateShotgunBullet.h"
#include "CreateARifleBullet.h"
#include "CreateSniperBullet.h"
#include "CreateFlameBullet.h"
#include "CreateGrenadeBullet.h"
#include "CreatePukeBullet.h"
#include "CreateTurretBullet.h"
#include "CreateBloodMsg.h"

#include "Spawner.h"
#include "SpawnManager.h"

#include "WeaponManager.h"
#include "CreateZombieMessage.h"
#include "CreateFastZombieMsg.h"
#include "CreateFatZombieMsg.h"
#include "CreateExplodingZombieMsg.h"
#include "CreateTankZombieMsg.h"
#include "CreateTurretMessage.h"
#include "BitmapFont.h"

#include "EntityManager.h"
#include "BaseObject.h"
#include "MovingObject.h"
#include "Player.h"
#include "Zombie.h"
#include "FastZombie.h"
#include "FatZombie.h"
#include "ExplodingZombie.h"
#include "TankZombie.h"
#include "BloodSplatter.h"

#include "Turret.h"
#include "Bullet.h"
#include "PickUp.h"
#include "Weapon.h"

#include "BehaviorManager.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Frame.h"
#include "MapManager.h"

#include <Windows.h>
#include <cstdlib>
#include <cassert>
#define WIN32_LEAN_AND_MEAN

/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ HTPGameState* HTPGameState::GetInstance(void)
{
	static HTPGameState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void HTPGameState::Enter(void)
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black

	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	// Initializ the Behavior Manager
	BehaviorManager::GetInstance()->Initialize();


	/**************************/
	// Load the assets
	/**************************/
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();
	SGD::InputManager*		pInput = SGD::InputManager::GetInstance();
	AnimationManager*		pAnimationManager = AnimationManager::GetInstance();

	// player animations
	pAnimationManager->Load("resource/config/animations/PlayerAnimation.xml", "player");
	pAnimationManager->Load("resource/config/animations/FlameThrower.xml", "flameThrowerRound");
	pAnimationManager->Load("resource/config/animations/testLandMine.xml", "testLandmine");
	pAnimationManager->Load("resource/config/animations/barbwireAnimation.xml", "testBarbwire");
	pAnimationManager->Load("resource/config/animations/sandbagAnimation.xml", "testSandbag");

	pAnimationManager->Load("resource/config/animations/Bullet.xml", "bullet");
	pAnimationManager->Load("resource/config/animations/Player_Death.xml", "playerDeath");
	pAnimationManager->Load("resource/config/animations/Landmine_Animation.xml", "landmine");

	m_hReticleImage = pGraphics->LoadTexture("resource/graphics/crosshair.png");


	// enemy animations

	pAnimationManager->Load("resource/config/animations/Zombie_Animation1.xml", "slowZombie");
	pAnimationManager->Load("resource/config/animations/Zombie_Animation2.xml", "fastZombie");
	pAnimationManager->Load("resource/config/animations/TankZombie.xml", "tankZombie");
	pAnimationManager->Load("resource/config/animations/ExplodingZombie.xml", "explodingZombie");
	pAnimationManager->Load("resource/config/animations/Explosion_Animation1.xml", "explosion");

	pAnimationManager->Load("resource/config/animations/FatZombie.xml", "fatZombie");
	pAnimationManager->Load("resource/config/animations/AcidAnimation.xml", "puke");
	pAnimationManager->Load("resource/config/animations/AcidAnimation.xml", "puke");

	//Blood Animation
	pAnimationManager->Load("resource/config/animations/BloodAnimations/blood1.xml", "blood1");
	pAnimationManager->Load("resource/config/animations/BloodAnimations/blood2.xml", "blood2");
	pAnimationManager->Load("resource/config/animations/BloodAnimations/blood3.xml", "blood3");
	pAnimationManager->Load("resource/config/animations/BloodAnimations/blood4.xml", "blood4");


	// other animations
	pAnimationManager->Load("resource/config/animations/Turret_Animation2.xml", "turret");
	pAnimationManager->Load("resource/config/animations/House_Animation.xml", "house");

	//pAnimationManager->Load("resource/config/animations/PowerCoreAnimation.xml",	"powerCore");

	//pAnimationManager->Load("resource/config/animations/StimPack.xml",				"stimPack");
	playerHurt1 = pAudio->LoadAudio("resource/audio/player_grunt1.wav");
	playerHurt2 = pAudio->LoadAudio("resource/audio/player_grunt2.wav");
	playerHurt3 = pAudio->LoadAudio("resource/audio/player_grunt3.wav");
	if (m_bStoryMode == true)
		MapManager::GetInstance()->LoadLevel(Game::GetInstance()->GetStoryProfile(), m_pEntities);
	else
		MapManager::GetInstance()->LoadLevel(Game::GetInstance()->GetSurvivalProfile(), m_pEntities);

	SpawnManager::GetInstance()->LoadFromFile("resource/config/levels/waves.txt");

	if (m_bIsChoiceScreen == true)
	{
		SpawnManager::GetInstance()->Activate();
	}
	
	// Music
	storyMusic = pAudio->LoadAudio("resource/audio/AmbienceDrama.xwm");
	survivalMusic = pAudio->LoadAudio("resource/audio/AmbienceDungeon.xwm");
	//m_bStoryMode == true ? pAudio->PlayAudio(storyMusic, true) : pAudio->PlayAudio(survivalMusic, true);
	pAudio->PlayAudio(m_bStoryMode == true ? storyMusic : survivalMusic, true);

	// SFX
	playerDeath = pAudio->LoadAudio("resource/audio/player_death1.wav");
	cannot_use_skill = pAudio->LoadAudio("resource/audio/cannotUseAbility7.wav");
	footstep = pAudio->LoadAudio("resource/audio/FootstepsWood.wav");
	//m_hWpnSwitch = pAudio->LoadAudio("resource/audio/switchweapon.wav");
	//m_hWaveChange = pAudio->LoadAudio("resource/audio/wavechange.wav");

	//m_hHudWpn = Game::GetInstance()->m_hHudWpn;
	//turretfire			= pAudio->LoadAudio("resource/audio/TurretFire.wav");

	zombie_pain = pAudio->LoadAudio("resource/audio/zombie_howl.wav");
	bullet_hit_zombie = pAudio->LoadAudio("resource/audio/bullet_hit_zombie.wav");
	bullet_hit_house = pAudio->LoadAudio("resource/audio/bullet_hit_zombie.wav");
	out_of_ammo = pAudio->LoadAudio("resource/audio/out_of_ammo.wav");
	reload_begin = pAudio->LoadAudio("resource/audio/reload_begin.wav");
	reload_finish = pAudio->LoadAudio("resource/audio/reload_finish.wav");
	explosion = pAudio->LoadAudio("resource/audio/Splode2.wav");
	vomit_hit_player = pAudio->LoadAudio("resource/audio/splat.wav");

	pistol_fire = pAudio->LoadAudio("resource/audio/pistol_fire.wav");
	shotgun_fire = pAudio->LoadAudio("resource/audio/shotgun_fire.wav");
	rifle_fire = pAudio->LoadAudio("resource/audio/rifle_fire.wav");
	sniper_fire = pAudio->LoadAudio("resource/audio/sniper_fire.wav");
	flamethrower_fire = pAudio->LoadAudio("resource/audio/fire_ignite_1.wav");
	smg_fire = pAudio->LoadAudio("resource/audio/smg_fire_1.wav");
	vomit_fire = pAudio->LoadAudio("resource/audio/vomit.wav");


	//m_hMain = &Game::GetInstance()->m_hMainTheme;
	//m_hSurvive = &Game::GetInstance()->m_hSurvivalTheme;

	// Setup the camera
	camera.SetSize({ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() });

	// Create the main entities
	m_pPlayer = CreatePlayer();
	m_pEntities->AddEntity(m_pPlayer, EntityBucket::BUCKET_PLAYER);

	MovingObject * pPlayer = dynamic_cast<MovingObject*>(m_pPlayer);

	WeaponManager::GetInstance()->Initialize(*pPlayer);

	m_tCompleteWave.AddTime(3);
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void HTPGameState::Exit(void)
{
	SGD::Event gameOverMsg = { "GAME_OVER", nullptr, this };
	gameOverMsg.SendEventNow();


	/**************************/
	// Unload the assets
	/**************************/
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();
	SGD::InputManager*		pInput = SGD::InputManager::GetInstance();

	pGraphics->UnloadTexture(MapManager::GetInstance()->GetMapTexture());
	pGraphics->UnloadTexture(m_hReticleImage);
	//pGraphics->UnloadTexture(m_hHudWpn);
	pGraphics->UnloadTexture(m_hReticleImage);



	if (pAudio->IsAudioPlaying(storyMusic) == true)
		pAudio->StopAudio(storyMusic);
	if (pAudio->IsAudioPlaying(survivalMusic) == true)
		pAudio->StopAudio(survivalMusic);

	pAudio->UnloadAudio(storyMusic);
	pAudio->UnloadAudio(survivalMusic);

	pAudio->UnloadAudio(playerDeath);
	pAudio->UnloadAudio(cannot_use_skill);
	pAudio->UnloadAudio(footstep);
	//pAudio->UnloadAudio(m_hWpnSwitch);
	pAudio->UnloadAudio(zombie_pain);
	pAudio->UnloadAudio(bullet_hit_zombie);
	pAudio->UnloadAudio(bullet_hit_house);
	pAudio->UnloadAudio(out_of_ammo);
	pAudio->UnloadAudio(reload_begin);
	pAudio->UnloadAudio(reload_finish);
	pAudio->UnloadAudio(explosion);
	//pAudio->UnloadAudio(m_hWaveChange);
	pAudio->UnloadAudio(vomit_hit_player);

	pAudio->UnloadAudio(pistol_fire);
	pAudio->UnloadAudio(shotgun_fire);
	pAudio->UnloadAudio(rifle_fire);
	pAudio->UnloadAudio(sniper_fire);
	pAudio->UnloadAudio(flamethrower_fire);
	pAudio->UnloadAudio(smg_fire);
	pAudio->UnloadAudio(vomit_fire);
	pAudio->UnloadAudio(playerHurt1);
	pAudio->UnloadAudio(playerHurt2);
	pAudio->UnloadAudio(playerHurt3);

	
	//pAudio->UnloadAudio(*m_hMain);
	//pAudio->UnloadAudio(*m_hSurvive);

	camera.SetTarget(nullptr);




	// Terminate the Behavior Manager
	BehaviorManager::GetInstance()->Terminate();

	WeaponManager::GetInstance()->Exit();

	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;

	if (m_pPlayer != nullptr)
		m_pPlayer->Release();

	m_pPlayer = nullptr;


	// Shutdown & release the Map & Animation Managers
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();

	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	MapManager::GetInstance()->UnloadLevel();
	AnimationManager::GetInstance()->Shutdown();
}
/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool HTPGameState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	if (m_bIsChoiceScreen == true)
	{
		if (pInput->IsKeyPressed(SGD::Key::RightArrow) == true)
		{
			m_nCursor++;

			if (m_nCursor > 1)
			{
				m_nCursor = 0;
			}
		}

		if (pInput->IsKeyPressed(SGD::Key::LeftArrow)==true)
		{
			m_nCursor--;

			if (m_nCursor < 0)
			{
				m_nCursor = 1;
			}
		}

		if (pInput->IsKeyPressed(SGD::Key::Enter)== true)
		{
			switch (m_nCursor)
			{
				case 0:
				{
						  m_bIsChoiceScreen = false;
						  return true;
						
				}
					break;

				case 1:
				{
						  if (GameplayState::GetInstance()->GetGameMode() == true)
						  {
							  Game::GetInstance()->RemoveState();
						      Game::GetInstance()->AddState(IntroState::GetInstance());
							  return true;
						  }

						  else
						  {
							  Game::GetInstance()->RemoveState();
							  Game::GetInstance()->AddState(GameplayState::GetInstance());
							  return true;
						  }
				}
					break;
			}
		}
	}

	else
	{
		/**********************************************************/
		// Press Escape to enter Pause menu
		/**********************************************************/
		if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsButtonDown(0, 9) == true)
		{
			SGD::Event msg("PAUSE");
			msg.SendEventNow();
			Game::GetInstance()->AddState(PauseState::GetInstance());
		}

		//WeaponManager::GetInstance()->Input();

		/**********************************************************/
		// Player Died!
		/**********************************************************/
		int numframes = AnimationManager::GetInstance()->GetAnimation("playerDeath")->GetFrames().size();
		numframes--;

		if (m_pPlayer->GetAnimation() == "playerDeath" && m_pPlayer->GetAnimationStamp().m_nCurrFrame == numframes)
		{
			SGD::Event msg("PAUSE");
			msg.SendEventNow();
			Game::GetInstance()->AddState(LoseGameState::GetInstance());


			if (pAudio->IsAudioPlaying(storyMusic) == true)
				pAudio->StopAudio(storyMusic);
			if (pAudio->IsAudioPlaying(survivalMusic) == true)
				pAudio->StopAudio(survivalMusic);
		}

		if (pInput->IsKeyPressed(SGD::Key::B) == true)
		{
			SGD::Event msg("PAUSE");
			msg.SendEventNow();

			//Calls the shopstate//
			Game::GetInstance()->AddState(ShopState::GetInstance());
		}


	}

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void HTPGameState::Update(float dt)
{
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	

	if (m_bIsChoiceScreen == true)
	{

	}

	else
	{
		SpawnManager * eSpawner = SpawnManager::GetInstance();
		WeaponManager::GetInstance()->Update(dt);

		if (player->isLevelCompleted() == false)
		{
			// Update the entities
			SpawnManager::GetInstance()->Update(dt);
			m_pEntities->UpdateAll(dt);


			// Check collisions
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_ENEMIES);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_BULLETS);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_PICKUPS);
			m_pEntities->CheckCollisions(BUCKET_ENEMIES, BUCKET_BULLETS);


			// Center camera on the player
			SGD::Point playerpos = m_pPlayer->GetPosition();
			playerpos.x -= Game::GetInstance()->GetScreenWidth() * 0.5f;
			playerpos.y -= Game::GetInstance()->GetScreenHeight() * 0.5f;
			camera.SetPostion(playerpos);


			// Process the events & messages
			SGD::EventManager::GetInstance()->Update();
			SGD::MessageManager::GetInstance()->Update();
			MapManager::GetInstance()->Update(dt);

			// Update the Map Manager
			//	MapManager::GetInstance()->Update(dt);

		}

		else
		{
			Game::GetInstance()->RemoveState();
			Game::GetInstance()->AddState(HTPGameState::GetInstance());
		}

		//For Testing enemies killed to enemies spawned for the current wave//
		//int numKilled = SpawnManager::GetInstance()->GetEnemiesKilled();

		if (SpawnManager::GetInstance()->GetEnemiesKilled() == SpawnManager::GetInstance()->GetNumWaveEnemies())
		{
			SpawnManager::GetInstance()->Deactivate();

			if (m_tCompleteWave.GetTime() > 0.0f)
			{
				m_tCompleteWave.Update(dt);
			}

			else if (SpawnManager::GetInstance()->GetCurrWave() == SpawnManager::GetInstance()->GetNumWaves() - 1)
			{
				SpawnManager::GetInstance()->SetGameWon(true);
			}

			else
			{
				m_bShopState = true;

				m_tNextWave.AddTime(6);
				m_tCompleteWave.AddTime(3);

				SGD::Event msg("PAUSE");
				msg.SendEventNow();

				//Calls the shopstate//
				Game::GetInstance()->AddState(ShopState::GetInstance());
			}
		}

		//ShopState::GetInstance()->Update(dt);
		m_tNextWave.Update(dt);
	}
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void HTPGameState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();
	const BitmapFont * pFont = Game::GetInstance()->GetFont();
	
	if (m_bIsChoiceScreen == true)
	{
		pFont->Draw("YES", { Game::GetInstance()->GetScreenWidth() / 2 - 100, Game::GetInstance()->GetScreenHeight() / 2 }, 1.0f, { 100, 0, 0 });
		pFont->Draw("NO", { Game::GetInstance()->GetScreenWidth() / 2 + 100, Game::GetInstance()->GetScreenHeight() / 2 }, 1.0f, { 100, 0, 0 });

		if (m_nCursor == 0)
		{
			pFont->Draw("()", { Game::GetInstance()->GetScreenWidth() / 2 - 125, Game::GetInstance()->GetScreenHeight() / 2 }, 1.0f, { 0, 100, 0 });
		}

		if (m_nCursor == 1)
		{
			pFont->Draw("()", { Game::GetInstance()->GetScreenWidth() / 2 + 75, Game::GetInstance()->GetScreenHeight() / 2 }, 1.0f, { 0, 100, 0 });
		}
	}

	else
	{
		SGD::Point textPos({ Game::GetInstance()->GetScreenWidth() / 2, Game::GetInstance()->GetScreenHeight() / 2 });

		// Draw background
		MapManager::GetInstance()->Render();

		// Render the entities
		m_pEntities->RenderAll();

		// Draw status bars
		float top = 450;
		float left = 435;

		float tops = 475;
		float lefts = 435;

		WeaponManager::GetInstance()->Render();

		if (m_tNextWave.GetTime() > 0.0f && m_bShopState == false && SpawnManager::GetInstance()->GetGameWon() == false)
		{
			stringstream nWave;
			nWave << "Wave " << SpawnManager::GetInstance()->GetCurrWave() + 1;

			pFont->Draw(nWave.str().c_str(), textPos, 1.0f, { 155, 0, 0 });

			if (pAudio->IsAudioPlaying(m_hWaveChange) == false)
			{
				pAudio->PlayAudio(m_hWaveChange, false);
			}
		}

		else if (m_bShopState == false && SpawnManager::GetInstance()->GetEnemiesKilled() == SpawnManager::GetInstance()->GetNumWaveEnemies() && SpawnManager::GetInstance()->GetGameWon() == false)
		{
			stringstream nComplete;
			nComplete << "Wave " << SpawnManager::GetInstance()->GetCurrWave() + 1 << " Complete";

			pFont->Draw(nComplete.str().c_str(), textPos, 1.0f, { 155, 0, 0 });
		}

		else if (SpawnManager::GetInstance()->GetGameWon() == true)
		{
			stringstream gameWin;
			gameWin << "YOU WIN!";

			pFont->Draw(gameWin.str().c_str(), textPos, 1.0f, { 155, 0, 0 });


		}

		if (SpawnManager::GetInstance()->GetEnemiesKilled() == SpawnManager::GetInstance()->GetNumWaveEnemies())
		{

		}

		///////OLD Energy Bar & Stamina Bar Render///////
		/*
		pGraphics->DrawRectangle(energyRect, { 0, 0, 255 });

		SGD::Rectangle staminaRect = { lefts, tops, lefts + m_pPlayer->GetAttributes()->m_fCurrStamina / m_pPlayer->GetAttributes()->m_fMaxStamina * 150, tops + 25 };
		pGraphics->DrawRectangle(staminaRect, { 0, 255, 0 });
		*/

		stringstream moneyCount;
		//moneyCount << "$" << Game::GetInstance()->GetProfile().money;
		pFont->Draw(moneyCount.str().c_str(), { 20, Game::GetInstance()->GetScreenHeight() - 75 }, 2.0f, { 0, 255, 0 });



		// Draw the reticle
		SGD::Point	retpos = SGD::InputManager::GetInstance()->GetMousePosition();
		float		retscale = 0.8f;

		retpos.Offset(-32.0F * retscale, -32.0F * retscale);
		pGraphics->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });
	}

}

/**************************************************************/
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD
//		- does NOT have invoking object!!!
//		- must use singleton to access members
/*static*/ void HTPGameState::MessageProc(const SGD::Message* pMsg)
{
	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID())
	{
	default:
	case MessageID::MSG_UNKNOWN:
	{
								   OutputDebugStringW(L"Game::MessageProc - unknown message id\n");
	}
		break;


	case MessageID::MSG_DESTROY_OBJECT:
	{
										  const DestroyObjectMessage* pDestroyMsg = dynamic_cast<const DestroyObjectMessage*>(pMsg);
										  assert(pDestroyMsg != nullptr && "Game::MessageProc - MSG_DESTROY_OBJECT is not actually a DestroyObjectMessage");

										  BaseObject* ptr = pDestroyMsg->GetEntity();

										  //if (ptr->GetType() == BaseObject::OBJ_SLOW_ZOMBIE)
										  //{
											 // Game::GetInstance()->GetProfile().money += 20;
										  //}

										  //else if (ptr->GetType() == BaseObject::OBJ_FAST_ZOMBIE)
										  //{
											 // Game::GetInstance()->GetProfile().money += 25;
										  //}

										  //else if (ptr->GetType() == BaseObject::OBJ_EXPLODING_ZOMBIE)
										  //{
											 // Game::GetInstance()->GetProfile().money += 35;
										  //}

										  //else if (ptr->GetType() == BaseObject::OBJ_FAT_ZOMBIE)
										  //{
											 // Game::GetInstance()->GetProfile().money += 75;
										  //}

										  //else if (ptr->GetType() == BaseObject::OBJ_TANK_ZOMBIE)
										  //{
											 // Game::GetInstance()->GetProfile().money += 100;
										  //}

										  HTPGameState::GetInstance()->m_pEntities->RemoveEntity(ptr);
	}
		break;


	case MessageID::MSG_CREATE_PSTL_BLT:
	{
										   const CreatePistolBullet* pCreateBulletMsg = dynamic_cast<const CreatePistolBullet*>(pMsg);
										   HTPGameState::GetInstance()->CreateBullet(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_SHTGN_BLT:
	{
											const CreateShotgunBullet* pCreateBulletMsg = dynamic_cast<const CreateShotgunBullet*>(pMsg);
											HTPGameState::GetInstance()->CreateShotGunBullet(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_ASSRFLE_BLT:
	{
											  const CreateARifleBullet* pCreateBulletMsg = dynamic_cast<const CreateARifleBullet*>(pMsg);
											  HTPGameState::GetInstance()->CreateBullet(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_SNPR_BLT:
	{
										   const CreateSniperBullet* pCreateBulletMsg = dynamic_cast<const CreateSniperBullet*>(pMsg);
										   HTPGameState::GetInstance()->CreateSnipeBullet(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_FLAME:
	{
										const CreateFlameBullet* pCreateBulletMsg = dynamic_cast<const CreateFlameBullet*>(pMsg);
										HTPGameState::GetInstance()->CreateFireBullet(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_PUKE:
	{
									   const CreatePukeBullet* pCreateBulletMsg = dynamic_cast<const CreatePukeBullet*>(pMsg);
									   HTPGameState::GetInstance()->CreatePukeyBullet(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_NADE:
	{
									   const CreateGrenadeBullet* pCreateBulletMsg = dynamic_cast<const CreateGrenadeBullet*>(pMsg);
									   HTPGameState::GetInstance()->CreateGrenade(pCreateBulletMsg->GetOwner());
	}
		break;
	case MessageID::MSG_CREATE_SLOW_ZOMBIE:
	{
											  const CreateZombieMessage* pCreateBulletMsg = dynamic_cast<const CreateZombieMessage*>(pMsg);
											  HTPGameState::GetInstance()->CreateZombie(pCreateBulletMsg->GetOwner());

	}
		break;

	case MessageID::MSG_CREATE_FAST_ZOMBIE:
	{
											  const CreateFastZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateFastZombieMsg*>(pMsg);
											  HTPGameState::GetInstance()->CreateZombie(pCreateBulletMsg->GetOwner());

	}
		break;

	case MessageID::MSG_CREATE_FAT_ZOMBIE:
	{
											 const CreateFatZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateFatZombieMsg*>(pMsg);
											 HTPGameState::GetInstance()->CreateZombie(pCreateBulletMsg->GetOwner());

	}
		break;

	case MessageID::MSG_CREATE_TANK_ZOMBIE:
	{
											  const CreateTankZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateTankZombieMsg*>(pMsg);
											  HTPGameState::GetInstance()->CreateZombie(pCreateBulletMsg->GetOwner());

	}
		break;

	case MessageID::MSG_CREATE_EXPLODING_ZOMBIE:

	{
												   const CreateExplodingZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateExplodingZombieMsg*>(pMsg);
											  HTPGameState::GetInstance()->CreateZombie(pCreateBulletMsg->GetOwner());
	}
		break;
	
	case MessageID::MSG_CREATE_TURRET:
	{
										 const CreateTurretMessage* pCreateTurretMsg = dynamic_cast<const CreateTurretMessage*>(pMsg);
										 HTPGameState::GetInstance()->CreateTurret(pCreateTurretMsg->GetOwner());
	}
		break;

	case MessageID::MSG_CREATE_TURRET_BLT:
	{
											 const CreateTurretBullet* pCreateBulletMsg = dynamic_cast<const CreateTurretBullet*>(pMsg);
											 HTPGameState::GetInstance()->CreateTurretBullets(pCreateBulletMsg->GetOwner());
	}
		break;

	case MessageID::MSG_CREATE_BLOOD:
	{
										const CreateBloodMsg* pCreateBloodMsg = dynamic_cast<const CreateBloodMsg*>(pMsg);
										HTPGameState::GetInstance()->CreateBlood(pCreateBloodMsg->GetSpawnPos());
	}
		break;
	}

	/* Restore previous warning levels */
#pragma warning( pop )

}


/**************************************************************/
// Factory Methods:
BaseObject* HTPGameState::CreatePlayer(void)
{
	Player* player = new Player;
	player->SetPosition({ 200, 200 });
	player->SetRotation(0.0f);
	player->SetMoveSpeed(180.0f);
	player->RetrieveBehavior("playerController");
	player->SetAnimation("player");
	return player;
}

void HTPGameState::CreateBlood(SGD::Point pos)
{
	BloodSplatter* blood = new BloodSplatter;
	blood->SetPosition(pos);
	
	unsigned int choice = rand() % 4;

	switch (choice)
	{
	case 0:
		blood->SetAnimation("blood1");
		break;
	case 1:
		blood->SetAnimation("blood2");
		break;
	case 2:
		blood->SetAnimation("blood3");
		break;
	case 3:
		blood->SetAnimation("blood4");
		break;
	}
	
	m_pEntities->AddEntity(blood, EntityBucket::BUCKET_BLOOD);
	blood->Release();
	blood = nullptr;
}


void HTPGameState::CreatePickUp(int type, SGD::Point pos)
{
	PickUp* pickup = new PickUp;

	pickup->SetType(type);
	pickup->SetPosition(pos);

	//switch (type)
	//{
	//case BaseObject::OBJ_POWERCORE:
	//	pickup->SetAnimation("powerCore");
	//	break;

	//case BaseObject::OBJ_STIMPACK:
	//	pickup->SetAnimation("stimPack");
	//	break;



	m_pEntities->AddEntity(pickup, EntityBucket::BUCKET_PICKUPS);
	pickup->Release();
	pickup = nullptr;
}

void HTPGameState::CreateTurret(MovingObject* owner)
{
	Turret* turret = new Turret;

	float		pixel_offset = 100.0f;
	SGD::Vector	ownerpos = { owner->GetPosition().x, owner->GetPosition().y };
	SGD::Vector	turretposV = (owner->GetDirection() * pixel_offset) + ownerpos;
	SGD::Point	turretposP = { turretposV.x, turretposV.y };

	turret->SetPosition(turretposP);
	turret->SetRotation(owner->GetRotation());
	turret->SetAnimation("turret");
	turret->RetrieveBehavior("hostile");

	turret->SetOwner(owner);

	m_pEntities->AddEntity(turret, EntityBucket::BUCKET_TURRETS);
	turret->Release();
	turret = nullptr;
}

void HTPGameState::CreateBullet(Weapon* owner)
{

	Bullet* bullet = new Bullet;
	bullet->SetOwner(owner->GetOwner());
	bullet->SetPosition(owner->GetOwner()->GetPosition());
	SGD::Vector direction = owner->GetOwner()->GetDirection();
	float angle = ((rand() % (int)owner->GetBulletSpread() * 2) - (int)owner->GetBulletSpread()) *SGD::PI / 180.0f;

	direction.Rotate(angle);

	bullet->SetDirection(direction);
	bullet->SetRotation(owner->GetOwner()->GetRotation());
	bullet->SetDamage(owner->GetDamage());

	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("bullet");

	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;


}
void HTPGameState::CreateGrenade(Weapon* owner)
{

	Bullet* bullet = new Bullet;
	bullet->SetOwner(owner->GetOwner());
	bullet->SetPosition(owner->GetOwner()->GetPosition());
	SGD::Vector direction = owner->GetOwner()->GetDirection();



	bullet->SetDirection(direction);
	bullet->SetRotation(owner->GetOwner()->GetRotation());
	bullet->SetDamage(owner->GetDamage());

	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("bullet");
	bullet->SetMoveSpeed(owner->GetSpeed());
	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;


}
void HTPGameState::CreateFireBullet(Weapon* owner)
{

	Bullet* bullet = new Bullet;
	bullet->SetOwner(owner->GetOwner());
	bullet->SetPosition(owner->GetOwner()->GetPosition());
	SGD::Vector direction = owner->GetOwner()->GetDirection();
	float angle = ((rand() % (int)owner->GetBulletSpread() * 2) - (int)owner->GetBulletSpread()) *SGD::PI / 180.0f;

	direction.Rotate(angle);

	bullet->SetDirection(direction);
	bullet->SetRotation(owner->GetOwner()->GetRotation());

	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("flameThrowerRound");
	bullet->SetDamage(owner->GetDamage());
	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;


}
void HTPGameState::CreatePukeyBullet(Weapon* owner)
{

	Bullet* bullet = new Bullet;
	bullet->SetOwner(owner->GetOwner());
	bullet->SetPosition(owner->GetOwner()->GetPosition());
	SGD::Vector direction = owner->GetOwner()->GetDirection();
	float angle = ((rand() % (int)owner->GetBulletSpread() * 2) - (int)owner->GetBulletSpread()) *SGD::PI / 180.0f;

	direction.Rotate(angle);

	bullet->SetDirection(direction);
	bullet->SetRotation(owner->GetOwner()->GetRotation());
	bullet->SetType(BaseObject::ObjectType::OBJ_VOMIT);

	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("puke");
	bullet->SetDamage(owner->GetDamage());
	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;


}
void HTPGameState::CreateShotGunBullet(Weapon* owner)
{
	for (unsigned int i = 0; i < 10; i++)
	{
		Bullet* bullet = new Bullet;
		bullet->SetOwner(owner->GetOwner());
		bullet->SetPosition(owner->GetOwner()->GetPosition());
		SGD::Vector direction = owner->GetOwner()->GetDirection();
		float angle = ((rand() % (int)owner->GetBulletSpread() * 2) - (int)owner->GetBulletSpread()) *SGD::PI / 180.0f;

		direction.Rotate(angle);

		bullet->SetDirection(direction);
		bullet->SetRotation(owner->GetOwner()->GetRotation());
		bullet->SetDamage(owner->GetDamage());


		bullet->SetVelocity(direction * owner->GetSpeed());
		bullet->SetAnimation("bullet");

		m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
		bullet->Release();
		bullet = nullptr;
	}
}
void HTPGameState::CreateARBullet(Weapon* owner)
{
	Bullet* bullet = new Bullet;
	bullet->SetRotation(owner->GetOwner()->GetRotation());
	bullet->SetOwner(owner->GetOwner());
	bullet->SetPosition(owner->GetOwner()->GetPosition());
	SGD::Vector direction = owner->GetOwner()->GetDirection();
	direction.Rotate(owner->GetRecoilTimer().GetTime()*Game::GetInstance()->DeltaTime());

	bullet->SetDirection(direction);
	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("bullet");
	bullet->SetDamage(owner->GetDamage());


	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;
}
void HTPGameState::CreateSnipeBullet(Weapon* owner)
{
	Bullet* bullet = new Bullet;
	bullet->SetRotation(owner->GetOwner()->GetRotation());
	bullet->SetOwner(owner->GetOwner());
	bullet->SetPosition(owner->GetOwner()->GetPosition());
	SGD::Vector direction = owner->GetOwner()->GetDirection();
	direction.Rotate(owner->GetRecoilTimer().GetTime()*Game::GetInstance()->DeltaTime());
	bullet->SetDamage(owner->GetDamage());
	bullet->SetDirection(direction);
	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("bullet");
	bullet->SetDamage(owner->GetDamage());


	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;
}
void HTPGameState::CreateTurretBullets(Turret* turret)
{
	Bullet* bullet = new Bullet;
	bullet->SetRotation(turret->GetRotation());
	bullet->SetOwner(turret);
	bullet->SetPosition(turret->GetPosition());
	SGD::Vector direction = turret->GetOwner()->GetDirection();
	direction.Rotate(turret->GetRecoilTimer().GetTime()*Game::GetInstance()->DeltaTime());
	
	bullet->SetDirection(direction);
	bullet->SetVelocity(direction * turret->GetSpeed());
	bullet->SetAnimation("bullet");
	bullet->SetDamage(turret->GetDamage());


	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;
}

void HTPGameState::CreateZombie(Spawner* owner)
{
	Zombie* zombie = new Zombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetHealth((float)INT_MAX);

	zombie->SetAnimation("slowZombie");
	zombie->SetMoveSpeed(64.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}

