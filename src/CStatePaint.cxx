#include "NGCmd.h"
#include "CCluster.h"
#include "CStatePaint.h"

void CStatePaint::enter(unsigned long long	t_ticks){

	/*
	 *	Check if User is using mouse and if not then set cursor to
	 *	last known position.
	 */

	if(!CInput::isMouse())
		CInput::setCursorPosition
			(m_cursorPositionX,
			 m_cursorPositionY);

	/*
	 *	Check that cursor is within the cluster window as resizing the
	 *	screen can offset the last known cursor position.
	 */

	if(CInput::getEvent()->cursor.x<getbegx(CScreen::getFrame(FRAME_MIDDLE)->window)
	|| CInput::getEvent()->cursor.x>getmaxx(CScreen::getFrame(FRAME_MIDDLE)->window))
		CInput::getEvent()->cursor.x=getbegx(CScreen::getFrame(FRAME_MIDDLE)->window);

	if(CInput::get()->e.cursor.y<getbegy(CScreen::getFrame(FRAME_MIDDLE)->window)
	|| CInput::get()->e.cursor.y>getmaxy(CScreen::getFrame(FRAME_MIDDLE)->window))
		CInput::get()->e.cursor.y=getbegy(CScreen::getFrame(FRAME_MIDDLE)->window);

	/*
	 *	Show cursor on screen.
	 */

	CInput::showCursor(true);
}

void CStatePaint::exit(unsigned long long	t_ticks){

	/*
	 *	Hide the cursor if User is only using the keys.
	 */

	wmouse_trafo(CScreen::getFrame(FRAME_MIDDLE)->window,&m_cursorPositionY,&m_cursorPositionX,1);
	if(!CInput::isMouse()) CInput::showCursor(false);
}

void CStatePaint::update(unsigned long long t_ticks){

	/*
	 *	The cursor positions are offset to align with 2D cluster matrix
	 *	and then the corresponding cell is retrieved.
	 */

	m_cursorPositionY*=GameData::getFactor();
	m_cursorPositionX*=.5;

	m_currentCell=GameOfLife::getCluster()->getCell(m_cursorPositionY+m_cursorPositionX);

	/*
	 *	Determine how to deal with each cell (this is for mouse support.)
	 */

	if(isActive)
		switch(m_brush){

			case	BRUSH_ERASE:	if(m_currentCell->getIsExtant())  m_currentCell->setIsExtant(false);	break;
			case	BRUSH_PAINT:	if(!m_currentCell->getIsExtant()) m_currentCell->setIsExtant(true);		break;
			case	BRUSH_NEUTRAL:	;break;
		}
}

void CStatePaint::flipCell(int y, int x){

	/*
	 *	Invert current cell state.
	 */

	m_currentCell->setIsExtant(!m_currentCell->getIsExtant());
}

void CStatePaint::onInput(Event* e){

	WINDOW* window=CScreen::getFrame(FRAME_MIDDLE)->window;

	m_cursorPositionX=e->cursor.x;
	m_cursorPositionY=e->cursor.y;

	if(wenclose(window,e->cursor.y,e->cursor.x)){

		wmouse_trafo(window,&m_cursorPositionY,&m_cursorPositionX,0);

		switch(e->key)
		{
			case KEY_LEFT:{

				e->cursor.x-=2;

				if(e->cursor.x<getbegx(window))

					/*
					 *	Offset by 2 to account for cell size and zero-index.
					 */

					e->cursor.x=getmaxx(window)+getbegx(window)-2;
			} break;

			case KEY_RIGHT:{

				e->cursor.x+=2;

				if(e->cursor.x>=getmaxx(window)+getbegx(window))
					e->cursor.x=getbegx(window);
			} break;

			case KEY_UP:{

				--e->cursor.y;

				if(e->cursor.y<getbegy(window))

					/*
					 *	Offset by 1 to account for zero-index.
					 */

					e->cursor.y=getmaxy(window)+getbegy(window)-1;
			} break;

			case KEY_DOWN:{

				++e->cursor.y;

				if(e->cursor.y>=getmaxy(window)+getbegy(window))
					e->cursor.y=getbegy(window);
			} break;

			case 32: flipCell(e->cursor.y,e->cursor.x);		break;

			case 27:{

				GCmd::set_mode_pause();
				CInput::showCursor(false);
			}
			break;

			case KEY_ENTER:
			case 10: GameOfLife::setState(MODE_PLAY); break;
			case KEY_MOUSE:{

				if(e->mse.bstate&BUTTON1_PRESSED){

					isActive=true;

					if(e->mse.bstate&BUTTON1_DOUBLE_CLICKED)
						flipCell(e->cursor.y,e->cursor.x);

					if(!e->mse.bstate&BUTTON_CTRL){

						m_brush=(BRUSH_ERASE);
					} else
						m_brush=(BRUSH_PAINT);
				}


				if(e->mse.bstate&BUTTON1_RELEASED
				|| getmouse(&e->mse)==OK)
					isActive=false;
			}
		};
	}
}
