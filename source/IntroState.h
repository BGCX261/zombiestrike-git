#pragma once

#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio

class IntroState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor
	static IntroState* GetInstance( void );

	
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
	IntroState( void )			= default;
	virtual ~IntroState( void )	= default;

	IntroState( const IntroState& )				= delete;	
	IntroState& operator= ( const IntroState& )	= delete;



	/**********************************************************/
	// Assets
	SGD::HTexture	m_hBackgroundImage	= SGD::INVALID_HANDLE;

};

