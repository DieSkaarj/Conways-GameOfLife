#include "NGCmd.h"
#include "NUtil.h"
#include "CScreen.h"

void CScreen::GUI::init(){

	makeMenu(CScreen::getHeight(),m_sidePanelWidth,0,0);
	makeDish(GameData::getFactor(),GameData::getFactor(),
			 CScreen::getHeight(),CScreen::getWidth());
	makeInfo(CScreen::getHeight(),m_sidePanelWidth,0,0);

	m_frames[FRAME_MESSAGES]->window=nullptr;
	m_frames[FRAME_MESSAGES]->panel=nullptr;

	makeCursor();

	if(menu) menu->destroy();

	menu=new CMenu(m_frames[FRAME_LEFT]);
	data=new CData();
}

void CScreen::GUI::makeMenu(int h,int w,int y,int x){

	SFrame* frame=m_frames[FRAME_LEFT];

	std::string	title=" COMMANDS ";

	WINDOW* window=newwin(h,w,y,x);

	x=NUtil::center(static_cast<int>(title.size()),w);

	wbkgd(window,CR_BLACK_WHITE);
	wborder(window,0,0,0,0,0,0,0,0);

	mvwaddch(window,0,x-1,ACS_RTEE);
	mvwaddch(window,0,x+title.size(),ACS_LTEE);

	wattron(window,CR_RED_WHITE);
	mvwprintw(window,0,x,"%s",title.c_str());
	wattroff(window,CR_RED_WHITE);

	frame->window=window;
	frame->panel=new_panel(frame->window);
}

void CScreen::GUI::makeDish(int h,int w,int y,int x){

	SFrame* frame=m_frames[FRAME_MIDDLE];

	/*
	 *	Get co-ordinates to center window according to the cluster size.
	 */

	w*=2;
	y-=(y+h)*.5;
	x-=(x+w)*.5;

	frame->window=newwin(h,w,y,x);
	frame->panel=new_panel(frame->window);
}

void CScreen::GUI::makeInfo(int h,int w,int y,int x){

	SFrame* frame=m_frames[FRAME_RIGHT];

	std::string title=" DATA ";

	x+=COLS-w;

	WINDOW* window=newwin(h,w,y,x);

	wbkgd(window,CR_BLACK_WHITE) ;

	x=NUtil::center(static_cast<int>(title.size()),w);

	wborder(window,0,0,0,0,0,0,0,0);

	mvwaddch(window,0,x-1,ACS_RTEE);
	mvwaddch(window,0,x+title.size(),ACS_LTEE);

	wattron(window,CR_RED_WHITE);
	mvwprintw(window,0,x,"%s",title.c_str());
	wattroff(window,CR_RED_WHITE);

	frame->window=window;
	frame->panel=new_panel(frame->window);
}

void CScreen::GUI::impDisplayMsg(std::vector<std::string> t_msg){

	SFrame* frame=getMessageFrame();

	/*
	 *	Lambda compares sizes and returns biggest
	 */

	auto compareSizeStr=[](int a,int b){

		return(a>b)?a:b;
	};

	/*
	 *	Cleanup any pre-existing message windows
	 */

	if(frame){

		CScreen::destroy(frame);
	}

	std::string halt=" Press ANY key ";

	int 		longest=0;
	int			len=0,
				size=t_msg.size();
	
	int			scrW=CScreen::getWidth(),
				scrH=CScreen::getHeight();

	/*
	 *	Find the longest string value.
	 *
	 *	Tabs will be replaced with five spaces.
	 */

	{
		for(auto& str:t_msg){

			len=str.size();

			for(auto& ch:str){
		
				if(ch=='\t') len+=5;
			}

			longest=compareSizeStr(longest,len);
		}	

		len=longest;
		len	+=MSG_BORDER_X_SPACING;
		size+=MSG_BORDER_Y_SPACING;
	}

	WINDOW* window;
	frame->window=window=newwin(size,len,
						 NUtil::center(size,scrH),
						 NUtil::center(len,scrW));
	frame->panel=new_panel(frame->window);

	for(size_t i=0;i<t_msg.size();++i)
		mvwprintw(window,
				 (MSG_BORDER_Y_SPACING*.5)+i,MSG_BORDER_X_SPACING*.5,
				  "%s",t_msg[i].c_str());

	wbkgd(window,CR_BLACK_WHITE);
	wborder(window,0,0,0,0,0,0,0,0);

	int y=size-1,x=len-halt.size()-2;

	mvwaddch(window,y,x-1,ACS_RTEE);
	mvwaddch(window,y,x+halt.size(),ACS_LTEE);
	mvwprintw(window,size-1,x,halt.c_str());
	
	t_msg.clear();
	
	int e=0;
	while(e<=1){

		e=getch();

		update_panels();
		doupdate();
	}
	
	CScreen::destroy(frame);
}

void CScreen::GUI::makeCursor(){

	SFrame* frame=m_frames[FRAME_CURSOR];

	WINDOW* window=newwin(1,1,0,0);
	waddch(window,ACS_CKBOARD);
	wbkgd(window,CR_BLUE_WHITE);

	frame->window=window;
	frame->panel=new_panel(frame->window);
}

void CScreen::GUI::destroy(){

	for(auto& f:m_frames){

		if(f){

			CScreen::destroy(f);
		}
	}
	
	menu=nullptr;
	data=nullptr;
	delete menu;
	delete data;
}

void CScreen::GUI::onInput(Event* e){

	if(m_bHasFocus)
		menu->onInput(e);

	if(wenclose(CScreen::getFrame(FRAME_MIDDLE)->window,e->cursor.y,e->cursor.x)
	&& GameOfLife::getCurrentGameState()->stateType!=MODE_PAINT&&e->mse.bstate&BUTTON1_PRESSED)
		GameOfLife::setState(MODE_PAINT);
}

void CScreen::GUI::draw(){

	data->draw(m_frames[FRAME_RIGHT]->window);
}
