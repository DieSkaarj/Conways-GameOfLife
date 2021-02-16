#include "CGame.h"
#include "CInput.h"

void CInput::init(){

	/*
	 *	Initialise NCurses input values.
	 */

	keypad(stdscr,true);
	nodelay(stdscr,true);
	ESCDELAY=5;
	mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION,nullptr);
	mouseinterval(5);

	/*
	 * Hide cursor.
	 */

	showCursor(false);
}

void CInput::impUpdate(unsigned long long t_dt){

	e.key=getch();

	if(isMouse()&&getmouse(&e.mse)==OK){

		setCursorPosition(e.mse.x,e.mse.y);
		showCursor(true);
	}

	if(e.key==KEY_RESIZE)
		CScreen::onResize();

	GameOfLife::onInput(&e);

	setCursorPosition(e.cursor.x,e.cursor.y);
}
