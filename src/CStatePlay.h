/************************************************************************/
/*																		*/
/*																		*/
/*	Class: CState														*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include "CState.h"

class CGameStates;

class CStatePlay:public CState{

	public:

		CStatePlay(CGameStates* owner):
		m_owner(owner){}

		void			enter(unsigned long long dt);
		void			exit(unsigned long long dt);
		void			update(unsigned long long dt);
		void			onInput(Event* event);

		CGameStates*	getOwner()	{return m_owner;}

		float			timerA=0;
		float			timerB=0;

	private:

		CGameStates*	m_owner;
};
