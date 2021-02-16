#pragma once

#include <array>
#include <menu.h>

struct SFrame;
struct Event;

class CMenu{

	public:

		enum EMenu{

			MENU_FILE_OPTS=0,
			MENU_SPEED_OPTS
		};

		CMenu(){}
		CMenu(SFrame*& frame){init(frame);}
		~CMenu();

		void		init(SFrame* frame);
		void		onInput(Event* e);
		void		destroy();

		MENU*		getMenu(int t_mId){return m_menus[t_mId];}
		MENU*		getCurrentMenu(){return m_currentMenu;}

		void		setMenuColors(MENU* menu,int back,int fore);	
		void		setCurrentMenu(int menu){m_currentMenu=m_menus[menu];}
		void		setCurrentMenu(MENU* p_menu){m_currentMenu=p_menu;}

	private:

		MENU*				m_currentMenu;
		ITEM**				m_speedOpts;
		ITEM**				m_progOpts;
		ITEM**				m_gameOpts;
		std::array<MENU*,2>	m_menus;
};
