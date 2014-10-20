#pragma once

#include "IGameState.h"

class LoseGameState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor
	static LoseGameState* GetInstance( void );

	
	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter( void )				override;	// load resources
	virtual void	Exit ( void )				override;	// unload resources

	virtual bool	Input( void )				override;	// handle user input
	virtual void	Update( float elapsedTime )	override;	// update entites
	virtual void	Render( void )				override;	// render entities / menu


private:
	/**********************************************************/
	// SINGLETON!
	LoseGameState( void )			= default;
	virtual ~LoseGameState( void )	= default;

	LoseGameState( const LoseGameState& )				= delete;	
	LoseGameState& operator= ( const LoseGameState& )	= delete;



	/**********************************************************/
	// Cursor Index
	int		m_nCursor		= 0;

};

