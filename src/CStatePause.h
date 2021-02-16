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

class CStatePause:public CState{

	public:

		CStatePause(CGameStates* owner):
		m_owner(owner){}

		CGameStates*	getOwner(){return m_owner;}

		void			enter(unsigned long long ticks);
		void			exit(unsigned long long ticks);
		void			update(unsigned long long ticks);
		void			onInput(Event* event);

	private:

		bool			m_step=false;
		CGameStates* 	m_owner;
};
