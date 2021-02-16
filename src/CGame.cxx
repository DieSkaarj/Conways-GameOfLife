#include <cassert>
#include <fstream>
#include <algorithm>

#include "CGame.h"
#include "AStrings.h"
#include "CCluster.h"

void GameOfLife::impInit(){

	/*
	 *	Initialise the dish with a fresh cluster, set the speed to
	 *	a predefined value and tell the program it is running.
	 * 
	 *	We also define what characters represent the cells state. Cells
	 *  have only two states DEAD and ALIVE.
	 */

	m_cluster=new CCluster(GameData::getFactor());
	m_speed=(GameData::getSpeed());
	m_bIsRunning=true;

	setCellDisplayType(CELL_DEAD,std::pair(' ',' '));
	setCellDisplayType(CELL_ALIVE,std::pair(' ',' '));
}

void GameOfLife::impUpdate(unsigned long long t_dt){

	/*
	 *	The current game state is directed to its own update function
	 *	and the gui is updated with the game data to display.
	 */

	CGameStates::update(t_dt);
}

void GameOfLife::impOnInput(Event* e){

	/*
	 *	The menu belongs to the CScreen class.
	 */

	CScreen::onInput(e);

	/*
	 *	There are some commands that should not be exclusive to the game
	 *	state. They are declared here.
	 */

	if(e->key==KEY_F(5))
		toggleGrid();

	if(e->key==KEY_F(1)&&CScreen::GUI::getMessageFrame()->window==nullptr)
		CScreen::GUI::promptMessage(vstrHelp);

	/*
	 *	Each game state has commands exclusive to its functionality.
	 *	These are described in the Class State (CState*) files.
	 */

	getCurrentGameState()->state->onInput(e);
}

void GameOfLife::impDraw(WINDOW* t_window){

	/*
	 *	The window must be cleared before any drawing is made. The
	 *	cluster colours are then retrieved from the game state. 
	 *	They are used to communicate what mode the game is in.
	 */

	werase(t_window);

	int deadColor=getCurrentGameState()->colors.first,
		liveColor=getCurrentGameState()->colors.second;

	/*
	 *	A loop looks at each cell and sets its colour and character
	 *	according to a predefined table.
	 */

	for(auto& n:*m_cluster->getCells()){

		if(n.getIsExtant()){

			wattron(t_window,A_DIM);
			wattron(t_window,COLOR_PAIR(liveColor));
			waddch(t_window,m_cellStateDesc[CELL_ALIVE].first);
			waddch(t_window,m_cellStateDesc[CELL_ALIVE].second);
			wattroff(t_window,COLOR_PAIR(liveColor));
			wattroff(t_window,A_DIM);
		}else{

			wattron(t_window,A_BOLD);
			wattron(t_window,COLOR_PAIR(deadColor));
			waddch(t_window,m_cellStateDesc[CELL_DEAD].first);
			waddch(t_window,m_cellStateDesc[CELL_DEAD].second);
			wattroff(t_window,COLOR_PAIR(deadColor));
			wattroff(t_window,A_BOLD);
		}
	}
}

void GameOfLife::toggleGrid(){

	/*
	 *	Text characters are used to create a guide.
	 */

	if(m_cellStateDesc[CELL_DEAD].second==' ') m_cellStateDesc[CELL_DEAD].second='.';
	else m_cellStateDesc[CELL_DEAD].second=' ';
}

void GameOfLife::setCellDisplayType(ECellState ecd,std::pair<char,char> chrs){

	/*
	 *	Copy a char-pair to the characters used to represent the cluster.
	 */

	m_cellStateDesc[ecd]=chrs;
}

void GameOfLife::impSetSpeed(int t_speed){

	/*
	 *	Overall speed is limited to a factor between 0-100.
	 */

	if(t_speed<0)	t_speed=0;
	if(t_speed>100)	t_speed=100;

	/*
	 *	If the speed is zero evolution is paused and the game state is
	 *	changed to reflect that.
	 */

	if(t_speed==0) setState(MODE_PAUSE);

	/*
	 *	Game interal speed is updated along side the games config.
	 */

	m_speed=t_speed;
	GameData::setSpeed(m_speed);
}

void GameOfLife::rules(){

	/*
	 *	Make a copy of the current cluster.
	 */

	VCluster nextGen(*m_cluster->getCells());

	/*
	 *	Implementation of the games' conditions for life and death.
	 *	Test against the current gens cells and change the nextGen
	 *	accordingly.
	 */

	for(int i=0;i<m_cluster->getSize();++i){

		switch(m_cluster->getCell(i)->getIsExtant()){

			case true:{

				if(m_cluster->getLivingNeighbours(i)>1
				&& m_cluster->getLivingNeighbours(i)<=3)
					nextGen[i].setIsExtant(true);
				else
					nextGen[i].setIsExtant(false);
			}	break;

			case false:{

				if(m_cluster->getLivingNeighbours(i)==3)
					nextGen[i].setIsExtant(true);
			}	break;
		}
	}

	/*
	 *	Set the next generation of cells in the cluster.
	 */

	m_cluster->setNextGen(&nextGen);
}

void GameOfLife::impReset(){

	/*
	 *	Resize the on-screen windows & panels and then create a new
	 *	cluster to be populated. The cycles are set to zero and the
	 *	game mode is set to Paused awaiting User input.
	 */

	CScreen::onResize();
	m_cluster=new CCluster(GameData::getFactor());
	m_cycle=0;
	setState(MODE_PAUSE);
}

void GameOfLife::impLoad(std::string savnm){

	int	factor,cycle,speed;
	std::string line;

	/*
	 *	Ensure that there is no whitespace in the Filename.
	 */

	savnm.erase(std::remove(savnm.begin(),savnm.end(),' '),savnm.end());

	/*
	 *	Append and prefix the string to include the save path.
	 */

	std::string path="./";
	path+=savnm;

	/*
	 *	Open the file and make sure it exists.
	 */

	std::ifstream load(path);
	assert(load.is_open());

	/*
	 *	This function will find the first new declaration of an equals
	 *	sign and store it in the given string.
	 */

	auto getValue=[&](int& val){

		std::getline(load,line,'=');
		std::getline(load,line);
		val=std::stoi(line);
	};

	/*
	 *	Declare a new vector of cells to store the incoming cluster to.
	 */

	VCluster cluster;

	/*
	 *	Loop through the file and retrieve the data from the save to
	 * 	recreate it.
	 *
	 *	When dealing with the cluster only the live cells are noted
	 *	as cells are instanced as non-extant on initialisation.
	 *	While, since the save file resembles the cluster in HRF* we 
	 *	need to account for newlines when reading it back in.
	 * 
	 *  *Human Readable Format
	 */

	while(std::getline(load,line)){

		if(line=="[data]"){

			getValue(factor);
			getValue(cycle);
			getValue(speed);
		}

		if(line=="[cluster]"){

			for(int i=0;i<(factor*factor);++i){

				CCell cell;

				std::getline(load,line,',');

				if(line=="1"||line=="\n1") cell.setIsExtant(true);

				cluster.push_back(cell);
			}
		}
	}

	/*
	 *	If the Sq. factor of the cluster is greater than the terminal
	 *	size a message is displayed and the load is aborted.
	 */

	if(factor>LINES){
		
		CScreen::GUI::promptMessage(vstrUnableToLoad);
		return;
	}

	/*
	 *	The relevant data is set to the values stored in the save file
	 *	and recreated in the game.
	 *
	 *	The factor must be set first to accommodate the incoming data.
	 */

	GameData::setFactor(factor);

	impReset();

	m_cluster->setNextGen(&cluster);
	setSpeed(speed);
	setCycle(cycle);

	/*
	 *	Set the game mode to Paused so that the User has control over
	 *	when to start the evolution of the cells.
	 */

	setState(MODE_PAUSE);
}

void GameOfLife::impSave(std::string savnm){

	/*
	 *	Remove all whitespace from the Savename string.
	 */

	savnm.erase(std::remove(savnm.begin(),savnm.end(),' '),savnm.end());

	/*	
	 *	If no name is given: dispay error message and return without saving.
	 *	NCurses automagically adds spaces to the field buffer so we do this
	 *	after removing all whitespace because the User may try to use a space 
	 * 	for a name.
	 */

	if(savnm==""){

		CScreen::GUI::promptMessage(vstrNoNameGiven);
		return;
	}

	/*
	 *	Affix ".sav" to the end of the Savename	and create a save path.
	 */

	savnm+=".sav";
	std::string path="./";
	path+=savnm;

	/*
	 *	Open the filestream that we will write the save to.
	 */

	std::ofstream save(path);
	assert(save.is_open());

	/*
	 *	The data is streamed into the file under their respective attributes.
	 *	The data header saves the evolution of the cell cluster while the cluster
	 *	data stores the state of each cell.
	 * 
	 *	There are better ways to store the cell data but a custom CSV type will suffice.
	 */

	save<<"/********************************************************************/\n";
	save<<"/*								Saved game							*/\n";
	save<<"/********************************************************************/\n\n";
	save<<"[data]\n";
	save<<"factor="	<<GameData::getFactor()	<<"\n";
	save<<"cycle="	<<getCycle()			<<"\n";
	save<<"speed="	<<GameData::getSpeed()	<<"\n";
	save<<"\n";
	save<<"[cluster]\n";

	for(auto& cell:*m_cluster->getCells()){

		switch(cell.getIsExtant()){

			case true:	save<<"1";	break;
			case false:	save<<"0";	break;
		}

		save<<",";

		if(((cell.getId())%m_cluster->getFactor())==0) save<<"\n";
	}

	/*
	 *	Close the file and end the stream.
	 */

	save.close();
}

void GameOfLife::destroy(){

	/*
	 *	The cluster is deleted when the game ends.
	 */

	m_cluster=nullptr;
	delete m_cluster;
}
