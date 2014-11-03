#ifndef GAME_H
#define GAME_H

//#include "StateMachine.h"
#include <stack>
#include "GamerProfile.h"
#include "../SGD Wrappers/SGD_Handle.h"

/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class BitmapFont;
class IGameState;
class MainMenuState;
class GameplayState;
class HowToPlayState;
class CreditsState;
class OptionsState;


/**************************************************************/
// Game class
//	- runs the game logic
//	- controls the SGD wrappers
//	- SINGLETON!
//		- there is only ONE instance
//		- global access method (GetInstance)
class Game
{
public:
	/**********************************************************/
	// Singleton Accessors:
	static Game*	GetInstance		( void );
	static void		DeleteInstance	( void );

	
	/**********************************************************/
	// Setup, Play, Cleanup:
	bool Initialize	( float width, float height, const wchar_t* title );
	int	 Update		( void );
	void Terminate	( void );
	
	
	/**********************************************************/
	// Screen Size Accessors:
	float				GetScreenWidth	( void ) const	{	return m_fScreenWidth;	}
	float				GetScreenHeight ( void ) const	{	return m_fScreenHeight;	} 


	/**********************************************************/
	// Font Accessor: (#include "BitmapFont.h" to use!)
	const BitmapFont*	GetFont			( void ) const	{	return m_pFont;			}


	/**********************************************************/
	// Delta time
	float				DeltaTime		( void ) const { return elapsedTime; }


	/**********************************************************/
	// Gamer profile
	GamerProfile&		GetProfile		( void )		{	return profiles[selectedProfile];	}
	void				LoadProfiles	( void );
	void				CreateStoryProfiles	( void );
	void				CreateSurvivalProfiles(void);


	GamerProfile		profiles[3];
	unsigned int		selectedProfile	= 0;



	/**********************************************************/
	// Game State Machine:
	//	- can ONLY be called by the state's Input, Update, or Render methods!!!
	void				AddState		( IGameState* pNewState );
	void				RemoveState		( void );



private:
	/**********************************************************/
	// Singleton Object:
	static Game*	s_pInstance;

	Game( void )	= default;		// default constructor
	~Game( void )	= default;		// destructor

	Game( const Game& )				= delete;	// copy constructor
	Game& operator= ( const Game& )	= delete;	// assignment operator

	

	/**********************************************************/
	// Screen Size
	float					m_fScreenWidth	= 1;
	float					m_fScreenHeight = 1;
	

	/**********************************************************/
	// Game title
	const wchar_t*			m_strGameTitle	= L"untitled";


	/**********************************************************/
	// Game Font
	BitmapFont*				m_pFont			= nullptr;


	/**********************************************************/
	// Current Game State
	IGameState*				m_pCurrState	= nullptr;
	std::stack<IGameState*>	stateMachine;


	/**********************************************************/
	// Game Time
	unsigned long			m_ulGameTime	= 0;
	float					elapsedTime		= 0.0f;


	/**********************************************************/
	// Gamer profile


	/**********************************************************/
	// Loading screen
	SGD::HTexture			loadScreen		= SGD::INVALID_HANDLE;

};

#endif //GAME_H
