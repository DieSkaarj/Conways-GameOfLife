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
#include "CScreen.h"

class CCell;

class CStatePaint:public CState{

	public:

		enum EBrush{

			BRUSH_NEUTRAL=0,
			BRUSH_PAINT,
			BRUSH_ERASE
		};

	public:

		CStatePaint(CGameStates* owner):
		m_owner(owner),m_brush(BRUSH_PAINT){}

		CGameStates*	getOwner(){return m_owner;}

		void			enter(unsigned long long ticks);
		void			exit(unsigned long long ticks);
		void			update(unsigned long long ticks);

		void			onInput(Event* event);
		void			setBrush(EBrush style);
		void			flipCell(int y,int x);

	private:

		CGameStates*	m_owner=nullptr;
		EBrush			m_brush=BRUSH_NEUTRAL;
		bool			isActive=false;
		int				m_cursorPositionX=getbegx(CScreen::getFrame(FRAME_MIDDLE)->window),
						m_cursorPositionY=getbegy(CScreen::getFrame(FRAME_MIDDLE)->window);
		CCell*			m_currentCell=nullptr;
};

inline void CStatePaint::setBrush(EBrush t_style){

	m_brush=t_style;
}
