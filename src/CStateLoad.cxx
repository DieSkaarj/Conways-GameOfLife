#include <dirent.h>

#include "CGame.h"
#include "NTime.h"
#include "CStateLoad.h"

void CStateLoad::enter(unsigned long long dt){

	m_vField.clear();

	/*
	 *	Stop input from manipulating the menu.
	 */

	CScreen::setMenuFocus(false);

	/*
	 *	Get the global message frame and assign a new panel & window. 
	 */

	SFrame* newFrame=CScreen::GUI::getMessageFrame();
	newFrame->window=newwin(18,40,((18+LINES)*.5)-18,((40+COLS)*.5)-40);
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
	mvwprintw(win,0,3,"%s"," Load ");
	mvwaddch(win,0,9,ACS_LTEE);

	/*
	 *	Open the app directory and filter files for save types.
	 *
	 *	Save file names into a new NCurses field buffer then insert 
	 *	the field into a vector.
	 */

	int y=0;

	DIR* dirp = opendir("./");
	struct dirent* dp;
	while ((dp=readdir(dirp))!=nullptr){

		std::string str=dp->d_name;
		std::size_t found=str.find(".sav");

		if(found!=std::string::npos) 
		{
			FIELD* f;
			f=new_field(1,30,y,3,0,0);
			set_field_buffer(f,0,dp->d_name);
			set_field_opts(f,O_VISIBLE|O_ACTIVE|O_PUBLIC);
			m_vField.push_back(f);

			++y;
		}

		m_vField.push_back(nullptr);
	}

	/*
	 *	Close directory.
	 */

	closedir(dirp);

	/*
	 *	Create a new form with the filename vector then post it to the
	 *	screen.
	 *
	 *	new_form takes a FIELD** so the vector is substituted by referencing 
	 *	the first entry of its internal dynamic array.
	 */

	m_form=new_form(&m_vField[0]);

	set_form_win(m_form,win);
	set_form_sub(m_form,derwin(win,getmaxy(win)-4,getmaxx(win)-4,2,2));

	post_form(m_form);
}

void CStateLoad::update(unsigned long long dt){

}

void CStateLoad::exit(unsigned long long dt){

	/*
	 *	Unpost and destory the form, free the fields and destroy the frame.
	 */

	unpost_form(m_form);

	free_form(m_form);
	m_form=nullptr;
	delete m_form;

	for(auto& field:m_vField){

		free_field(field);
		field=nullptr;
		delete field;
	}

	m_vField.clear();

	CScreen::destroy(CScreen::GUI::getMessageFrame());

	/*
	 *	Allow input to be directed to the main menu and set the game state
	 *	to paused.
	 */

	CScreen::setMenuFocus(true);
	GameOfLife::setState(MODE_PAUSE);
}

void CStateLoad::onInput(Event* e){

	/*
	 *	Lambda highlights selected field in form.
	 */

	auto select=[&](int option){

		set_field_back(current_field(m_form),A_NORMAL);
		form_driver(m_form,option);
		set_field_back(current_field(m_form),A_STANDOUT);
	};

	switch(e->key){

		case KEY_UP:	select(REQ_PREV_FIELD);			break;
		case KEY_DOWN:	select(REQ_NEXT_FIELD);			break;
		case KEY_ENTER:
		case 10:{

			if(field_buffer(current_field(m_form),0)!=nullptr)
				GameOfLife::load(field_buffer(current_field(m_form),0));

			exit(NTime::getTicks());
		}
		break;
		case 27:		exit(NTime::getTicks());		break;
		default:		form_driver(m_form,e->key);	break;
	};
}
