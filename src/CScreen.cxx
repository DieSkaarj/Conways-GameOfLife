#include "CGame.h"
#include "CScreen.h"
#include "AStrings.h"

void CScreen::impInit(){

	/*
	 *	Initialise NCurses screen values and start and set colours
	 *	if available to terminal.
	 *
	 * 	Get terminal width and height from screen values.
	 */

	initscr();

	int w=COLS,h=LINES;
	bool cursSet=false;

	m_width=w;
	m_height=h;

	noecho();
	curs_set(cursSet);
	cbreak();

	start_color();
	init_pair(1,COLOR_WHITE,COLOR_GREEN);
	init_pair(2,COLOR_YELLOW,COLOR_CYAN);
	init_pair(3,COLOR_WHITE,COLOR_YELLOW);
	init_pair(4,COLOR_BLACK,COLOR_WHITE);
	init_pair(5,COLOR_WHITE,COLOR_BLACK);
	init_pair(6,COLOR_GREEN,COLOR_RED);
	init_pair(7,COLOR_RED,COLOR_WHITE);
	init_pair(8,COLOR_BLUE,COLOR_WHITE);
	init_pair(9,COLOR_WHITE,COLOR_BLUE);
	init_pair(10,COLOR_WHITE,COLOR_RED);

	wbkgd(stdscr,CR_WHITE_BLUE);

	gui=GUI::get();
}

void CScreen::impOnResize(){

	m_width=COLS;
	m_height=LINES;

	gui->destroy();
	gui->init();

	if(getHeight()<GameData::getFactor()||getWidth()<(GameData::getFactor()*2)+32){

		GUI::promptMessage(vstrResizeMsg);
		hide_panel(getFrame(FRAME_MIDDLE)->panel);
		hide_panel(getFrame(FRAME_RIGHT)->panel);
	}
	
	if(getHeight()<SIDEPANE_LENGTH||getWidth()<SIDEPANE_LENGTH){

		hide_panel(getFrame(FRAME_RIGHT)->panel);
	}

	top_panel(getFrame(FRAME_CURSOR)->panel);	
	CInput::showCursor(false);

	update_panels();
	doupdate();
}

void CScreen::impUpdate(){

	/*
	 *	Update cluster and gui
	 */

	GameOfLife::draw(getFrame(FRAME_MIDDLE)->window);
	gui->draw();

	/*
	 *	Update screen according to NCurses.
	 */

	update_panels();
	doupdate();
}

void CScreen::impShutdown(){

	/*
	 *	Destroy singleton and end NCurses.
	 */

	m_instance=nullptr;
	delete m_instance;

	endwin();
}

void CScreen::destroy(SFrame* f){

	/*
	 *	Helper function to destroy SFrames. 
	 */

	del_panel(f->panel);
	f->panel=nullptr;
	delete f->panel;
	delwin(f->window);
	f->window=nullptr;
	delete f->window;
	f=nullptr;
	delete f;
}
