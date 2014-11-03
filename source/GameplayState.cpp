#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "WinGameState.h"
#include "LoseGameState.h"
#include "ShopState.h"

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
#include "Spawner.h"
#include "SpawnManager.h"

#include "WeaponManager.h"
#include "CreateZombieMessage.h"
#include "CreateFastZombieMsg.h"
#include "CreateFatZombieMsg.h"
#include "CreateExplodingZombieMsg.h"
#include "CreateTankZombieMsg.h"



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
/*static*/ GameplayState* GameplayState::GetInstance( void )
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void GameplayState::Enter( void )
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {0, 0, 0} );	// black

	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize( &MessageProc );

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	// Initializ the Behavior Manager
	BehaviorManager::GetInstance()->Initialize();


	/**************************/
	// Load the assets
	/**************************/
	SGD::GraphicsManager*	pGraphics			= SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio				= SGD::AudioManager::GetInstance();
	SGD::InputManager*		pInput				= SGD::InputManager::GetInstance();
	AnimationManager*		pAnimationManager	= AnimationManager::GetInstance();

	// player animations
	pAnimationManager->Load("resource/config/animations/PlayerAnimation.xml", "player");
	pAnimationManager->Load("resource/config/animations/FlameThrower.xml", "flameThrowerRound");
	pAnimationManager->Load("resource/config/animations/Bullet.xml", "bullet");




	// enemy animations
	pAnimationManager->Load("resource/config/animations/Zombie_Animation1.xml", "zombie1");
	pAnimationManager->Load("resource/config/animations/Zombie_Animation2.xml", "zombie2");


	// other animations
	pAnimationManager->Load("resource/config/animations/Turret_Animation.xml",		"turret");
	//pAnimationManager->Load("resource/config/animations/PowerCoreAnimation.xml",	"powerCore");

	//pAnimationManager->Load("resource/config/animations/StimPack.xml",				"stimPack");

	MapManager::GetInstance()->LoadLevel(Game::GetInstance()->GetProfile(), m_pEntities);
	SpawnManager::GetInstance()->LoadFromFile("resource/config/levels/waves.txt");
	SpawnManager::GetInstance()->Activate();
	// Music


	// SFX
	playerDeath			= pAudio->LoadAudio("resource/audio/player_death1.wav");
	cannot_use_skill	= pAudio->LoadAudio("resource/audio/cannotUseAbility7.wav");
	footstep			= pAudio->LoadAudio("resource/audio/FootstepsWood.wav");
	m_hWpnSwitch		= pAudio->LoadAudio("resource/audio/switchweapon.wav");
	m_hWaveChange		= pAudio->LoadAudio("resource/audio/wavechange.wav");

	m_hHudWpn = pGraphics->LoadTexture("resource/graphics/hudweapons.png");
	//turretfire			= pAudio->LoadAudio("resource/audio/TurretFire.wav");

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
/*virtual*/ void GameplayState::Exit( void )
{
	SGD::Event gameOverMsg = { "GAME_OVER", nullptr, this };
	gameOverMsg.SendEventNow();


	/**************************/
	// Unload the assets
	/**************************/
	SGD::GraphicsManager*	pGraphics	= SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio		= SGD::AudioManager::GetInstance();
	SGD::InputManager*		pInput		= SGD::InputManager::GetInstance();

	
	pGraphics->UnloadTexture(MapManager::GetInstance()->GetMapTexture());
	pGraphics->UnloadTexture(m_hHudWpn);



	pAudio->UnloadAudio(playerDeath);
	pAudio->UnloadAudio(cannot_use_skill);
	pAudio->UnloadAudio(footstep);
	pAudio->UnloadAudio(m_hWpnSwitch);
	pAudio->UnloadAudio(m_hWaveChange);

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
	MapManager::GetInstance()->UnloadLevel();
	AnimationManager::GetInstance()->Shutdown();

	

	// Terminate & deallocate the SGD wrappers

	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();
}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool GameplayState::Input( void )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


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
	//int numframes = AnimationManager::GetInstance()->GetAnimation("playerDeath")->GetFrames().size();
	//numframes--;

	//if (m_pPlayer->GetAnimation() == "playerDeath" && m_pPlayer->GetAnimationStamp().m_nCurrFrame == numframes)
	//{
	//	SGD::Event msg("PAUSE");
	//	msg.SendEventNow();
	//	Game::GetInstance()->AddState(LoseGameState::GetInstance());
	//}
	





	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void GameplayState::Update( float dt )
{
	WeaponManager::GetInstance()->Update(dt);
	SpawnManager * eSpawner = SpawnManager::GetInstance();

	Player* player = dynamic_cast<Player*>(m_pPlayer);


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
		Game::GetInstance()->AddState(GameplayState::GetInstance());
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


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();
	const BitmapFont * pFont = Game::GetInstance()->GetFont();

	SGD::Point textPos({ Game::GetInstance()->GetScreenWidth() / 2, Game::GetInstance()->GetScreenHeight() / 2 });

	// Draw background
	MapManager::GetInstance()->Render();

	// Render the entities
	m_pEntities->RenderAll();

	// Draw status bars
	float top	= 450;
	float left	= 435;

	float tops	= 475;
	float lefts	= 435;

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
	
}

/**************************************************************/
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD
//		- does NOT have invoking object!!!
//		- must use singleton to access members
/*static*/ void GameplayState::MessageProc( const SGD::Message* pMsg )
{
/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch( pMsg->GetMessageID() )
	{
		default:
		case MessageID::MSG_UNKNOWN:
		{
			OutputDebugStringW( L"Game::MessageProc - unknown message id\n" );
		}
		break;
	
	
		case MessageID::MSG_DESTROY_OBJECT:
		{
			const DestroyObjectMessage* pDestroyMsg = dynamic_cast<const DestroyObjectMessage*>(pMsg);
			assert(pDestroyMsg != nullptr && "Game::MessageProc - MSG_DESTROY_OBJECT is not actually a DestroyObjectMessage");

			BaseObject* ptr = pDestroyMsg->GetEntity();
			GameplayState::GetInstance()->m_pEntities->RemoveEntity(ptr);
		}
		break;


		case MessageID::MSG_CREATE_PSTL_BLT:
		{
			const CreatePistolBullet* pCreateBulletMsg = dynamic_cast<const CreatePistolBullet*>(pMsg);
			GameplayState::GetInstance()->CreateBullet(pCreateBulletMsg->GetOwner());
		}
		break;
		case MessageID::MSG_CREATE_SHTGN_BLT:
		{
			const CreateShotgunBullet* pCreateBulletMsg = dynamic_cast<const CreateShotgunBullet*>(pMsg);
			GameplayState::GetInstance()->CreateShotGunBullet(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_ASSRFLE_BLT:
		{
			const CreateARifleBullet* pCreateBulletMsg = dynamic_cast<const CreateARifleBullet*>(pMsg);
			GameplayState::GetInstance()->CreateBullet(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_SNPR_BLT:
		{
			const CreateSniperBullet* pCreateBulletMsg = dynamic_cast<const CreateSniperBullet*>(pMsg);
			GameplayState::GetInstance()->CreateSnipeBullet(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_FLAME:
		{
			const CreateFlameBullet* pCreateBulletMsg = dynamic_cast<const CreateFlameBullet*>(pMsg);
			GameplayState::GetInstance()->CreateFireBullet(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_SLOW_ZOMBIE:
		{
												  const CreateZombieMessage* pCreateBulletMsg = dynamic_cast<const CreateZombieMessage*>(pMsg);
											GameplayState::GetInstance()->CreateZombie(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_FAST_ZOMBIE:
		{

												  const CreateFastZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateFastZombieMsg*>(pMsg);
											GameplayState::GetInstance()->CreateFastZombie(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_FAT_ZOMBIE:
		{
												 const CreateFatZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateFatZombieMsg*>(pMsg);
											GameplayState::GetInstance()->CreateFatZombie(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_EXPLODING_ZOMBIE:
		{
													   const CreateExplodingZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateExplodingZombieMsg*>(pMsg);
											GameplayState::GetInstance()->CreateExplodingZombie(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_TANK_ZOMBIE:
		{
												  const CreateTankZombieMsg* pCreateBulletMsg = dynamic_cast<const CreateTankZombieMsg*>(pMsg);
											GameplayState::GetInstance()->CreateTankZombie(pCreateBulletMsg->GetOwner());
		}
			break;

	}

/* Restore previous warning levels */
#pragma warning( pop )

}


/**************************************************************/
// Factory Methods:
BaseObject* GameplayState::CreatePlayer( void )
{
	Player* player = new Player;
	player->SetPosition({ 200, 200 });
	player->SetRotation(0.0f);
	player->SetMoveSpeed(180.0f);
	player->RetrieveBehavior("playerController");
	player->SetAnimation("player");
	return player;
}

void GameplayState::CreatePickUp( int type, SGD::Point pos )
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

void GameplayState::CreateTurret( SGD::Point pos, float rotation )
{
	Turret* turret = new Turret;
	turret->SetPosition(pos);
	turret->SetRotation(rotation);
	turret->SetAnimation("turret");
	turret->RetrieveBehavior("rotate");

	m_pEntities->AddEntity(turret, EntityBucket::BUCKET_TURRETS);
	turret->Release();
	turret = nullptr;
}

void GameplayState::CreateBullet(Weapon* owner)
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
void GameplayState::CreateFireBullet(Weapon* owner)
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
void GameplayState::CreateShotGunBullet(Weapon* owner)
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
void GameplayState::CreateARBullet(Weapon* owner)
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
void GameplayState::CreateSnipeBullet(Weapon* owner)
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

void GameplayState::CreateZombie(Spawner* owner)
{
	Zombie* zombie = new Zombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetAnimation("zombie1");
	zombie->SetMoveSpeed(64.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");
	
	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}
void GameplayState::CreateFatZombie(Spawner* owner)
{
	FatZombie* zombie = new FatZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetAnimation("zombie1");
	zombie->SetMoveSpeed(32.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}
void GameplayState::CreateFastZombie(Spawner* owner)
{
	FastZombie* zombie = new FastZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetAnimation("zombie1");
	zombie->SetMoveSpeed(96.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}

void GameplayState::CreateExplodingZombie(Spawner* owner)
{
	ExplodingZombie* zombie = new ExplodingZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetAnimation("zombie1");
	zombie->SetMoveSpeed(64.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}
void GameplayState::CreateTankZombie(Spawner* owner)
{
	TankZombie* zombie = new TankZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetAnimation("zombie1");
	zombie->SetMoveSpeed(32.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}