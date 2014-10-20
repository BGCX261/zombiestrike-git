#pragma once

#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"


/**************************************************************/
// MainMenuState class
//	- handles the main menu
//	- SINGLETON (statically allocated, not dynamic)
class MainMenuState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor
	static MainMenuState* GetInstance( void );
	
	
	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter( void )				override;	// load resources
	virtual void	Exit ( void )				override;	// unload resources

	virtual bool	Input( void )				override;	// handle user input
	virtual void	Update( float elapsedTime )	override;	// update entites
	virtual void	Render( void )				override;	// render entities / menu


	/**********************************************************/
	// Accessors & Mutators:
	const SGD::HTexture GetReticle(void) const	{ return m_hReticleImage; }


private:
	/**********************************************************/
	// SINGLETON!
	MainMenuState( void )			= default;
	virtual ~MainMenuState( void )	= default;

	MainMenuState( const MainMenuState& )				= delete;	
	MainMenuState& operator= ( const MainMenuState& )	= delete;



	/**********************************************************/
	// Cursor Index
	enum MenuItems { PLAY_GAME, HOW_TO_PLAY, OPTIONS, CREDITS, EXIT };

	int		m_nCursor		= 0;
	int		m_mPrevious		= 0;
	bool	clicked			= false;
	bool	selected		= false;



	/**********************************************************/
	// Assets
	SGD::HTexture	m_hBackgroundImage	= SGD::INVALID_HANDLE;
	SGD::HTexture	m_hTitleImage		= SGD::INVALID_HANDLE;

	SGD::HTexture	m_hReticleImage		= SGD::INVALID_HANDLE;
	SGD::HTexture	m_hButton1			= SGD::INVALID_HANDLE;
	SGD::HTexture	m_hButton2			= SGD::INVALID_HANDLE;

	SGD::HAudio		m_hButtonSwitchSFX	= SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMenuChangeSFX	= SGD::INVALID_HANDLE;


	/**********************************************************/
	// Selection rectangles stuff
	SGD::Rectangle*	selectonrects	= nullptr;
	int*			stringlengths	= nullptr;	// [NUM_CHOICES] = { 9, 11, 7, 7, 4 };
	float			starting_x		= 35.0F;
	float			starting_y		= 200.0F;
	float			vertical_offset	= 100.0F;
	float			text_scale		= 0.67F;
};
