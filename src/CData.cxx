#include "CData.h"
#include "CGame.h"
#include "NUtil.h"
#include "CCluster.h"

void CData::draw(WINDOW* t_window){

	auto cleanLineBetweenBorder=[&t_window](int y){

		for(int i=1;i<t_window->_maxx;++i)
			mvwprintw(t_window,y,i," ");
	};

	cleanLineBetweenBorder(2);
	cleanLineBetweenBorder(4);
	cleanLineBetweenBorder(5);
	cleanLineBetweenBorder(7);
	cleanLineBetweenBorder(8);
	cleanLineBetweenBorder(9);

	int center		=NUtil::center(static_cast<int>(GameOfLife::getCurrentGameState()->name.size()),static_cast<int>(getmaxx(t_window))),
		liveCount	=GameOfLife::getCluster()->getLiveCount(),
		totalCount	=GameOfLife::getCluster()->getSize(),
		deadCount	=totalCount-liveCount;

	wattron(t_window,COLOR_PAIR(4)|A_BOLD);
	mvwprintw(t_window,
			 2,center,
			 "%s",
			 GameOfLife::getCurrentGameState()->name.c_str());
	wattroff(t_window,COLOR_PAIR(4)|A_BOLD);

	mvwprintw(	t_window,4,2,"%s%i","Cycle: ",
				GameOfLife::getCycle());
	mvwprintw(	t_window,5,2,"%s%i","Speed: ",
				GameData::getSpeed());
	mvwprintw(	t_window,7,2,"%s%i","Live:  ",
				liveCount);
	mvwprintw(	t_window,8,2,"%s%i","Dead:  ",
				deadCount);
	mvwprintw(	t_window,9,2,"%s%i","Total: ",
				totalCount);
};
