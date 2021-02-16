#include "CGame.h"
#include "NTime.h"
#include "CStateSave.h"

void CStateSave::enter(unsigned long long dt){

	/*
	 *	Stop input from manipulating the menu.
	 */

	CScreen::setMenuFocus(false);

	/*
	 *	Get the global message frame and assign a new panel & window. 
	 */

	SFrame* newFrame=CScreen::GUI::getMessageFrame();
	newFrame->window=newwin(3,40,((3+LINES)*.5)-3,((40+COLS)*.5)-40);
	newFrame->panel=new_panel(newFrame->window);

	WINDOW* win=newFrame->window;

	/*
	 *	Accentuate the window with a box.
	 */

	box(win,0,0);

	/*
	 *	Set a stylised label with an offset of 2 over the window
	 */

	mvwaddch(win,0,2,ACS_RTEE);
	mvwprintw(win,0,3,"%s"," Save ");
	mvwaddch(win,0,9,ACS_LTEE);

	/*
	 *	Create a field to take in user input to give save a name.
	 */

	m_field=new_field(1,30,0,3,0,0);

	set_field_opts(m_field,O_VISIBLE|O_ACTIVE|O_EDIT|O_PUBLIC|O_STATIC);
	set_field_back(m_field,A_UNDERLINE);

	/*
	 *	Create a new form with the save input field and then post it to
	 *	the screen.
	 */

	m_form=new_form(&m_field);

	set_form_win(m_form,win);
	set_form_sub(m_form,derwin(win,getmaxy(win)-2,getmaxx(win)-2,1,1));

	post_form(m_form);
}

void CStateSave::update(unsigned long long dt){

}

void CStateSave::exit(unsigned long long dt){

	/*
	 *	Unpost and destory the form, free the field and destroy the frame.
	 */

	unpost_form(m_form);

	free_form(m_form);
	m_form=nullptr;
	delete m_form;

	free_field(m_field);
	m_field=nullptr;
	delete m_field;

	CScreen::destroy(CScreen::GUI::getMessageFrame());

	/*
	 *	Allow input to be directed to the main menu and set the game state
	 *	to paused.
	 */

	CScreen::setMenuFocus(true);
	GameOfLife::setState(MODE_PAUSE);
}

void CStateSave::onInput(Event* event){

	switch(event->key){

		case KEY_LEFT:	form_driver(m_form,REQ_PREV_CHAR);	break;
		case KEY_RIGHT:	form_driver(m_form,REQ_NEXT_CHAR);	break;
		case KEY_BACKSPACE:
		case '\b':		form_driver(m_form,REQ_DEL_PREV);	break;
		case KEY_DC:	form_driver(m_form,REQ_DEL_CHAR);	break;
		case 10:{

			GameOfLife::save(field_buffer(m_field,0));
			exit(NTime::getTicks());
		}
		break;
		case 27:		exit(NTime::getTicks()); 			break;
		default:		form_driver(m_form,event->key);		break;
	};
	
	form_driver(m_form,REQ_VALIDATION);
}
