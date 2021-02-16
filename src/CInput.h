#pragma once

#include <panel.h>
#include "CScreen.h"

struct Event{

	struct{
		int 	x{0},
				y{0};
	} cursor;
	static int		key;
	static MEVENT	mse;
};

inline int Event::key;
inline MEVENT Event::mse;

class CInput{

	public:

		CInput(CInput& other)=delete;
		void operator=(const CInput&)=delete;

		~CInput();

		static void		update(unsigned long long dt)	{get()->impUpdate(dt);}

	public:

		static void		setCursorPosition(int x,int y)	{get()->e.cursor.x=x;get()->e.cursor.y=y;move_panel(CScreen::getFrame(FRAME_CURSOR)->panel,get()->e.cursor.y,get()->e.cursor.x);}
		static void		showCursor(bool c)				{if(c) show_panel(CScreen::getFrame(FRAME_CURSOR)->panel);else if(!c) hide_panel(CScreen::getFrame(FRAME_CURSOR)->panel);}

		static bool		isMouse()						{return(get()->e.key==KEY_MOUSE)?true:false;}
		static int		getKey()						{return(get()->e.key);}
		static Event* 	getEvent()						{return(&get()->e);}

	public:

		static CInput*	get(){if(m_instance==nullptr)m_instance=new CInput;return m_instance;}

	private:

		CInput(){init();}

		void		impUpdate(unsigned long long dt);

		void		init();

	public:

		Event		e;

	private:

		static CInput*	m_instance;
};

inline CInput*	CInput::m_instance=nullptr;
