#pragma once

#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_String.h"
#include "GameCamera.h"
#include "MapManager.h"


/**************************************************************/
// Forward class declaration
class BaseObject;
class Weapon;
class Spawner;
class EntityManager;
class BehaviorManager;
class AnimationManager;



/**************************************************************/
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static GameplayState* GetInstance( void );


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter	( void )		override;	// load resources
	virtual void	Exit	( void )		override;	// unload resources
													
	virtual bool	Input	( void )		override;	// handle user input
	virtual void	Update	( float dt )	override;	// update game entities / animations
	virtual void	Render	( void )		override;	// render game entities / menus


	/**********************************************************/
	// Other Methods:
	GameCamera*		GetCamera		( void )	{ return &camera; }
	SGD::Size		GetWorldSize	( void )	{ return m_szWorldSize; }
	void			PauseAudio		( bool );


	/**********************************************************/
	// Factory Methods:
	BaseObject*		CreatePlayer	( void );
	void			CreateZombie	(Spawner* owner );
	void			CreateFatZombie(Spawner* owner);
	void			CreateFastZombie(Spawner* owner);
	void			CreateExplodingZombie(Spawner* owner);
	void			CreateTankZombie(Spawner* owner);


	void			CreatePickUp	( int type, SGD::Point pos );
	void			CreateTurret	( SGD::Point pos, float rotation );
	void			CreateBullet	( Weapon* owner );
	void			CreateShotGunBullet	(Weapon* owner);
	void			CreateARBullet	(Weapon* owner);
	void			CreateSnipeBullet	(Weapon* owner);
	void			CreateFireBullet(Weapon* owner);
	



	/**********************************************************/
	// Assets
	SGD::HAudio playerDeath			= SGD::INVALID_HANDLE;
	SGD::HAudio cannot_use_skill	= SGD::INVALID_HANDLE;
	SGD::HAudio footstep			= SGD::INVALID_HANDLE;
	SGD::HAudio turretfire			= SGD::INVALID_HANDLE;
	SGD::HAudio m_hWpnSwitch		= SGD::INVALID_HANDLE;

	SGD::HTexture m_hHudWpn = SGD::INVALID_HANDLE;

private:
	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	GameplayState( void )			= default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor

	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator


	/**********************************************************/
	// Message Callback Function:
	static void MessageProc( const SGD::Message* pMsg );



	/**********************************************************/
	// Game Entities
	enum EntityBucket { BUCKET_PLAYER, BUCKET_ENEMIES, BUCKET_BULLETS, BUCKET_PICKUPS, BUCKET_TURRETS };
	EntityManager*			m_pEntities			= nullptr;
	AnimationManager*		m_pAnimManager		= nullptr;

	//BaseObject*				m_pPlayer			= nullptr;
	BaseObject*					m_pPlayer			= nullptr;
	GameCamera				camera;


	/**********************************************************/
	// World size
	SGD::Size				m_szWorldSize		= { 1024, 768 };


	//Textures
	SGD::HTexture	m_hReticleImage = SGD::INVALID_HANDLE;

};
