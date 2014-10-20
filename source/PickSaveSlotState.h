#pragma once
#include "IGameState.h"
class PickSaveSlotState : public IGameState
{
public:

	static PickSaveSlotState* GetInstance(void);



	virtual void	Enter(void);
	virtual void	Exit(void);

	virtual bool	Input(void);
	virtual void	Update(float elapsedTime);
	virtual void	Render(void);


private:

	PickSaveSlotState(void) = default;
	virtual ~PickSaveSlotState(void) = default;

	PickSaveSlotState(const PickSaveSlotState&) = delete;
	PickSaveSlotState& operator= (const PickSaveSlotState&) = delete;


	
	enum MenuItems { SAVE1, SAVE2, SAVE3 , EXIT};


	int		m_nCursor = 0;
};

