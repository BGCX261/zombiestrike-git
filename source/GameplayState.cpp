#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "WinGameState.h"
#include "LoseGameState.h"

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

#include "Turret.h"
#include "Bullet.h"
#include "PickUp.h"
#include "Weapon.h"

#include "BehaviorManager.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Frame.h"
#include "MapManager.h"

#include <cstdlib>
#include <cassert>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


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



	// other animations
	pAnimationManager->Load("resource/config/animations/Turret_Animation2.xml",		"turret");
	//pAnimationManager->Load("resource/config/animations/PowerCoreAnimation.xml",	"powerCore");

	//pAnimationManager->Load("resource/config/animations/StimPack.xml",				"stimPack");

	MapManager::GetInstance()->LoadLevel(Game::GetInstance()->GetProfile(), m_pEntities);
	SpawnManager::GetInstance()->Activate();


	// Music
	storyMusic		= pAudio->LoadAudio("resource/audio/AmbienceDrama.xwm");
	survivalMusic	= pAudio->LoadAudio("resource/audio/AmbienceDungeon.xwm");
	//m_bStoryMode == true ? pAudio->PlayAudio(storyMusic, true) : pAudio->PlayAudio(survivalMusic, true);
	pAudio->PlayAudio(m_bStoryMode == true ? storyMusic : survivalMusic, true);


	// SFX
	playerDeath			= pAudio->LoadAudio("resource/audio/player_death1.wav");
	cannot_use_skill	= pAudio->LoadAudio("resource/audio/cannotUseAbility7.wav");
	footstep			= pAudio->LoadAudio("resource/audio/FootstepsWood.wav");
	m_hWpnSwitch		= pAudio->LoadAudio("resource/audio/switchweapon.wav");

	m_hHudWpn = pGraphics->LoadTexture("resource/graphics/hudweapons.png");
	//turretfire			= pAudio->LoadAudio("resource/audio/TurretFire.wav");

	zombie_pain			= pAudio->LoadAudio("resource/audio/zombie_howl.wav");
	bullet_hit_zombie	= pAudio->LoadAudio("resource/audio/bullet_hit_zombie.wav");
	bullet_hit_house	= pAudio->LoadAudio("resource/audio/bullet_hit_zombie.wav");
	out_of_ammo			= pAudio->LoadAudio("resource/audio/out_of_ammo.wav");
	reload_begin		= pAudio->LoadAudio("resource/audio/reload_begin.wav");
	reload_finish		= pAudio->LoadAudio("resource/audio/reload_finish.wav");
	explosion			= pAudio->LoadAudio("resource/audio/Splode2.wav");
	vomit_hit_player	= pAudio->LoadAudio("resource/audio/splat.wav");

	pistol_fire			= pAudio->LoadAudio("resource/audio/pistol_fire.wav");
	shotgun_fire		= pAudio->LoadAudio("resource/audio/shotgun_fire.wav");
	rifle_fire			= pAudio->LoadAudio("resource/audio/rifle_fire.wav");
	sniper_fire			= pAudio->LoadAudio("resource/audio/sniper_fire.wav");
	flamethrower_fire	= pAudio->LoadAudio("resource/audio/fire_ignite_1.wav");
	smg_fire			= pAudio->LoadAudio("resource/audio/smg_fire_1.wav");
	vomit_fire			= pAudio->LoadAudio("resource/audio/vomit.wav");



	// Setup the camera
	camera.SetSize({ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() });



	// Create the main entities

	m_pPlayer = CreatePlayer();
	m_pEntities->AddEntity(m_pPlayer, EntityBucket::BUCKET_PLAYER);

	MovingObject * pPlayer = dynamic_cast<MovingObject*>(m_pPlayer);
	
	WeaponManager::GetInstance()->Initialize(*pPlayer);

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
	pGraphics->UnloadTexture(m_hReticleImage);
	pGraphics->UnloadTexture(m_hHudWpn);
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
	pAudio->UnloadAudio(m_hWpnSwitch);
	pAudio->UnloadAudio(zombie_pain);
	pAudio->UnloadAudio(bullet_hit_zombie);
	pAudio->UnloadAudio(bullet_hit_house);
	pAudio->UnloadAudio(out_of_ammo);
	pAudio->UnloadAudio(reload_begin);
	pAudio->UnloadAudio(reload_finish);
	pAudio->UnloadAudio(explosion);
	pAudio->UnloadAudio(vomit_hit_player);

	pAudio->UnloadAudio(pistol_fire);
	pAudio->UnloadAudio(shotgun_fire);
	pAudio->UnloadAudio(rifle_fire);
	pAudio->UnloadAudio(sniper_fire);
	pAudio->UnloadAudio(flamethrower_fire);
	pAudio->UnloadAudio(smg_fire);
	pAudio->UnloadAudio(vomit_fire);


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
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


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
	





	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void GameplayState::Update( float dt )
{
	WeaponManager::GetInstance()->Update(dt);

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
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	

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

	///////OLD EnergyBar Render///////
	/*
	pGraphics->DrawRectangle(energyRect, { 0, 0, 255 });

	SGD::Rectangle staminaRect = { lefts, tops, lefts + m_pPlayer->GetAttributes()->m_fCurrStamina / m_pPlayer->GetAttributes()->m_fMaxStamina * 150, tops + 25 };
	pGraphics->DrawRectangle(staminaRect, { 0, 255, 0 });
	*/
	

	// Draw the reticle
	SGD::Point	retpos = SGD::InputManager::GetInstance()->GetMousePosition();
	float		retscale = 0.8f;

	retpos.Offset(-32.0F * retscale, -32.0F * retscale);
	pGraphics->DrawTexture(m_hReticleImage, retpos, 0.0F, {}, { 255, 255, 255 }, { retscale, retscale });
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
		case MessageID::MSG_CREATE_PUKE:
		{
										   const CreatePukeBullet* pCreateBulletMsg = dynamic_cast<const CreatePukeBullet*>(pMsg);
			GameplayState::GetInstance()->CreatePukeyBullet(pCreateBulletMsg->GetOwner());
		}
			break;
		case MessageID::MSG_CREATE_NADE:
		{
										   const CreateGrenadeBullet* pCreateBulletMsg = dynamic_cast<const CreateGrenadeBullet*>(pMsg);
											GameplayState::GetInstance()->CreateGrenade(pCreateBulletMsg->GetOwner());
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

		case MessageID::MSG_CREATE_TURRET:
		{
			const CreateTurretMessage* pCreateTurretMsg = dynamic_cast<const CreateTurretMessage*>(pMsg);
			GameplayState::GetInstance()->CreateTurret(pCreateTurretMsg->GetOwner());
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

void GameplayState::CreateTurret( MovingObject* owner )
{
	Turret* turret = new Turret;

	float		pixel_offset	= 100.0f;
	SGD::Vector	ownerpos		= { owner->GetPosition().x, owner->GetPosition().y };
	SGD::Vector	turretposV		= (owner->GetDirection() * pixel_offset) + ownerpos;
	SGD::Point	turretposP		= { turretposV.x, turretposV.y };

	turret->SetPosition(turretposP);
	turret->SetRotation(owner->GetRotation());
	turret->SetAnimation("turret");
	turret->RetrieveBehavior("hostile");

	turret->SetOwner(owner);

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
void GameplayState::CreateGrenade(Weapon* owner)
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
void GameplayState::CreatePukeyBullet(Weapon* owner)
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
	bullet->SetDamage(owner->GetDamage());
	bullet->SetDirection(direction);
	bullet->SetVelocity(direction * owner->GetSpeed());
	bullet->SetAnimation("bullet");
	bullet->SetDamage(owner->GetDamage());


	m_pEntities->AddEntity(bullet, EntityBucket::BUCKET_BULLETS);
	bullet->Release();
	bullet = nullptr;
}

void	GameplayState::CreateZombie(Spawner* owner)
{
	Zombie* zombie = new Zombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);

	zombie->SetAnimation("slowZombie");
	zombie->SetMoveSpeed(64.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");
	
	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}
void			GameplayState::CreateFatZombie(Spawner* owner)
{
	FatZombie* zombie = new FatZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);

	zombie->SetAnimation("fatZombie");
	zombie->SetMoveSpeed(32.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}
void			GameplayState::CreateFastZombie(Spawner* owner)
{
	FastZombie* zombie = new FastZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	
	zombie->SetAnimation("fastZombie");
	zombie->SetMoveSpeed(96.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}

void			GameplayState::CreateExplodingZombie(Spawner* owner)
{
	ExplodingZombie* zombie = new ExplodingZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);

	zombie->SetAnimation("explodingZombie");
	zombie->SetMoveSpeed(64.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}
void			GameplayState::CreateTankZombie(Spawner* owner)
{
	TankZombie* zombie = new TankZombie;
	zombie->SetPosition(owner->GetPosition());
	zombie->SetRotation(0.0f);
	zombie->SetAnimation("tankZombie");
	zombie->SetMoveSpeed(32.0f);
	zombie->SetTarget(m_pPlayer);
	zombie->RetrieveBehavior("runTo");

	m_pEntities->AddEntity(zombie, EntityBucket::BUCKET_ENEMIES);
	zombie->Release();
	zombie = nullptr;
}