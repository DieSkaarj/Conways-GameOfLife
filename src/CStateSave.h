/************************************************************************/
/*																		*/
/*																		*/
/*	Class: CState														*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include <form.h>

#include "CState.h"

class CGameStates;

class CStateSave:public CState{

	public:

		CStateSave(CGameStates* owner):
		m_owner(owner){}

		CGameStates*	getOwner(){return m_owner;}

		void			enter(unsigned long long ticks);
		void			exit(unsigned long long ticks);
		void			update(unsigned long long ticks);
		void			onInput(Event* event);

	private:

		FIELD*				m_field;
		FORM*				m_form;
		CGameStates* 		m_owner;
};
