#include <cstring>

#include "CMenu.h"
#include "NGCmd.h"

struct MenuOptions{

	std::array<std::pair<std::string,void(*)()>,6> ASpeedOpts{
	
		std::make_pair(" +1",GCmd::set_speed_up_1),
		std::make_pair("+10",GCmd::set_speed_up_10),
		std::make_pair("100",GCmd::set_speed_100),
		std::make_pair(" -1",GCmd::set_speed_dwn_1),
		std::make_pair("-10",GCmd::set_speed_dwn_10),
		std::make_pair("NIL",GCmd::set_speed_0)
	};

	std::array<std::pair<std::string,void(*)()>,10> AFileOpts{
	
		std::make_pair("New",GCmd::reset_cluster),
		std::make_pair(" ",nullptr),
		std::make_pair("Load",GCmd::load),
		std::make_pair("Save",GCmd::save),
		std::make_pair(" ",nullptr),
		std::make_pair("Start",GCmd::set_mode_run),
		std::make_pair("Pause",GCmd::set_mode_pause),
		std::make_pair("Create",GCmd::set_mode_pattern),
		std::make_pair(" ",nullptr),
		std::make_pair("Quit",GCmd::shutdown)
	};
};

inline MenuOptions opts;

CMenu::~CMenu(){

	destroy();
}

void CMenu::init(SFrame* frame){

	/*	Lambda creates and defines NCurses menu	*/

	auto makeMenu=[](MENU*& menu,WINDOW* t_win,ITEM** t_opts,int t_h,int t_w,int t_y,int t_x,int t_rows,int t_cols,int col_space){

		menu=new_menu(t_opts);

		menu->spc_cols=col_space;
		set_menu_win(menu,t_win);
		set_menu_sub(menu,derwin(t_win,t_h,t_w+(col_space),t_y,t_x));
		set_menu_mark(menu,"");
		set_menu_format(menu,t_rows,t_cols);
	};

	WINDOW* win=frame->window;

	int menuRows,menuCols;

	{
		static std::array<ITEM*,opts.AFileOpts.size()> progOptions;
		m_progOpts=&progOptions[0];

		std::string helpMsg="'F1' - Help";
		wattron(win,COLOR_PAIR(8));

		mvwprintw(	win,
					getmaxy(win)-2,										/* ensures that the message is always displayed above the border on the last line of the menu */
					((getmaxx(win)+helpMsg.size())*.5)-helpMsg.size(),
					helpMsg.c_str());
		wattroff(win,COLOR_PAIR(8));


		for(size_t i=0;i<opts.AFileOpts.size();++i){

			ITEM* it=new_item(opts.AFileOpts[i].first.c_str(),nullptr);
			progOptions[i]=it;

				set_item_userptr(progOptions[i],reinterpret_cast<void*>(opts.AFileOpts[i].second));
			}

		menuRows=opts.AFileOpts.size();
		menuCols=opts.AFileOpts.size()/menuRows;

		makeMenu(	m_menus[MENU_FILE_OPTS],win,m_progOpts,
					opts.AFileOpts.size(),6,
					2,((getmaxx(win)+6)*.5)-6,
					menuRows,menuCols,
					0);
	}

	{
		static std::array<ITEM*,opts.ASpeedOpts.size()> speedMenuItems;
		m_speedOpts=&speedMenuItems[0];

		std::string	label="Speed:";

		int MenuYOffset=getbegy(m_menus[0]->usersub)+opts.AFileOpts.size();

		wattron(win,COLOR_PAIR(8));
		mvwprintw(win,MenuYOffset+1,getbegx(win)+2,"%s",label.c_str());
		wattroff(win,COLOR_PAIR(8));

		for(size_t i=0;i<opts.ASpeedOpts.size();++i){

			ITEM* it=new_item(opts.ASpeedOpts[i].first.c_str(),nullptr);
			speedMenuItems[i]=it;

			set_item_userptr(speedMenuItems[i],reinterpret_cast<void*>(opts.ASpeedOpts[i].second));
		}

		menuCols=opts.ASpeedOpts.size()*.5;
		menuRows=opts.ASpeedOpts.size()/menuCols;

		makeMenu(	m_menus[MENU_SPEED_OPTS],win,m_speedOpts,
					opts.ASpeedOpts.size()*.5,
					4+(opts.ASpeedOpts[0].first.size())*2,
					MenuYOffset+2,getbegx(win)+2,
					menuRows,menuCols,
					1);
	}

	m_currentMenu=m_menus[MENU_FILE_OPTS];

	/* Post Menus */
	for(auto& t_menu:m_menus){

		post_menu(t_menu);
	}
}

void CMenu::onInput(Event* e){

	EGameState currentMode=GameOfLife::getCurrentGameState()->stateType;

	/*	Lamba tests the cursors current position against items	*
	 *  in the menus and sets focus to any item that collides	*/

	auto menuMseDetect=[](Event* e, MENU* menu){

		int offsetX=0,
			offsetY=0;

		if(menu->usersub){

			offsetX=getbegx(menu->usersub),
			offsetY=getbegy(menu->usersub);
		} else exit(1); 

		int itId=0;

			for(int r=0;r<menu->rows;++r){

				for(int c=0;c<menu->cols;++c){

					itId=(r*menu->cols)+c;

					if(e->cursor.y==offsetY+r
					&& e->cursor.x>=offsetX
					&& e->cursor.x<offsetX+strlen(item_name(menu->items[itId]))){

						set_current_item(menu,menu->items[itId]);
					}

				offsetX+=menu->namelen+menu->spc_cols;
				}

			offsetX=menu->usersub->_begx;
			}
	};

	/*	Lambda executes the item that has focus	*/

	auto executeMenuItem=[&](){

		ITEM* selected=current_item(m_currentMenu);
		void (*execute)();

		execute=reinterpret_cast <void(*)()>(item_userptr(selected));
		if(execute)
			execute();
	};

	/*	This ensures that only one menu option	*
	 *	is highlighted to the user at any time	*/

	for(auto& menu:m_menus){

		if(m_currentMenu==menu)
			setMenuColors(m_currentMenu,4,10);
		else
			setMenuColors(menu,4,4);
	}

	switch(e->key){

		case	KEY_UP:{

			/*	Items 0-2 corresponds to the top	*
			 *	row in the Speed Options menu		*/

			if((m_currentMenu==getMenu(MENU_SPEED_OPTS)
			&& current_item(m_currentMenu)==m_currentMenu->items[0])
			|| (m_currentMenu==getMenu(MENU_SPEED_OPTS)
			&& current_item(m_currentMenu)==m_currentMenu->items[1])
			|| (m_currentMenu==getMenu(MENU_SPEED_OPTS)
			&& current_item(m_currentMenu)==m_currentMenu->items[2])){

				setCurrentMenu(MENU_FILE_OPTS);
			}else if(currentMode!=MODE_PAINT)
				menu_driver(m_currentMenu,REQ_UP_ITEM);
		}
		break;
		case	KEY_DOWN:{

			if(m_currentMenu==getMenu(MENU_FILE_OPTS)
			&& current_item(m_currentMenu)==m_currentMenu->items[opts.AFileOpts.size()-1])
				setCurrentMenu(MENU_SPEED_OPTS);
			else if(currentMode!=MODE_PAINT)
				menu_driver(m_currentMenu,REQ_DOWN_ITEM);
		}
		break;
		case	KEY_LEFT:{

			if(currentMode!=MODE_PAINT)
				menu_driver(m_currentMenu,REQ_LEFT_ITEM);
		}
		break;
		case	KEY_RIGHT:{

			if(currentMode!=MODE_PAINT)
				menu_driver(m_currentMenu,REQ_RIGHT_ITEM);
		}
		break;

		case	10:{

			if(currentMode!=MODE_PAINT)
				executeMenuItem();
		} break;

		case	KEY_MOUSE:{

			for(auto& menu:m_menus){

				menuMseDetect(e,menu);

				if(wenclose(menu->usersub,e->cursor.y,e->cursor.x)){

					setCurrentMenu(menu);
					
					if(e->mse.bstate&BUTTON1_PRESSED){

						executeMenuItem();
					}
				}
			}
		} break;
	}
}

void CMenu::setMenuColors(MENU* t_menu,int t_back,int t_fore){

	set_menu_back(t_menu,COLOR_PAIR(t_back));
	set_menu_fore(t_menu,COLOR_PAIR(t_fore));	
}

void CMenu::destroy(){

	for(size_t i=0;i<opts.AFileOpts.size();++i)
		free_item(m_progOpts[i]);
	for(size_t i=0;i<opts.ASpeedOpts.size();++i)
		free_item(m_speedOpts[i]);

	for(auto &m:m_menus){

		unpost_menu(m);
		free_menu(m);
		m=nullptr;
	}
}
