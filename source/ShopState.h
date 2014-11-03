#pragma once
#include "IGameState.h"
#include "Timer.h"

class ShopState :public IGameState
{	
	
	ShopState() = default;
	virtual ~ShopState() = default;

	ShopState(const ShopState&) = delete;
	ShopState& operator=(const ShopState&) = delete;

	int m_nCursor = 0;

	//Shop time between waves
	Timer m_tShopTimer;

	bool m_bTimerSet = false;

public:

	static ShopState* GetInstance(void);

	virtual void	Enter(void)				override;
	virtual void	Exit(void)				override;

	virtual bool	Input(void)				override;
	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;

	Timer GetShopTimer() const { return m_tShopTimer; }

	void SetShopTimer(Timer sTimer) { m_tShopTimer = sTimer; }
};

