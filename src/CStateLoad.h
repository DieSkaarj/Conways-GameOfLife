/************************************************************************/
/*																		*/
/*																		*/
/*	Class: CState														*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include <vector>

#include <form.h>

#include "CState.h"

class CGameStates;

class CStateLoad:public CState{

	public:

		CStateLoad(CGameStates* owner):
		m_owner(owner){}

		CGameStates*	getOwner(){return m_owner;}

		void			enter(unsigned long long ticks);
		void			exit(unsigned long long ticks);
		void			update(unsigned long long ticks);
		void			onInput(Event* event);

	private:

		std::vector<FIELD*> m_vField={nullptr};
		FORM*				m_form=nullptr;
		CGameStates* 		m_owner=nullptr;
};
