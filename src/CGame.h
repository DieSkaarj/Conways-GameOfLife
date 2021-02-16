/************************************************************************/
/*																		*/
/*																		*/
/*	Class: GameOfLife													*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include <array>
#include <ncurses.h>

#include "Enums.h"
#include "CInput.h"
#include "CGameData.h"
#include "CGameState.h"

class CCluster;
struct SConfig;

typedef std::array<std::pair<char,char>,2> CellGlyphs;

class GameOfLife:public CGameStates{

	public:

		GameOfLife(GameOfLife &other)=delete;
		void operator=(const GameOfLife&)=delete;

	public:

		~GameOfLife(){destroy();}

	public:

		static void			init()							{CScreen::init();get()->impInit();}
		static bool			isRunning()						{return get()->m_bIsRunning;}
		static void			update(unsigned long long dt) 	{get()->impUpdate(dt);}
		static void			onInput(Event* event)			{get()->impOnInput(event);}
		static void			draw(WINDOW* window)			{get()->impDraw(window);}
		static void			reset()							{get()->impReset();}
		static void			shutdown()						{get()->m_bIsRunning=false;}
		static void			load(std::string savename)		{get()->impLoad(savename);}

		static int			getSpeed()						{return get()->m_speed;}
		static CCluster*	getCluster()					{return get()->m_cluster;}
		static int			getCycle()						{return get()->m_cycle;}
		static SStateInfo*	getCurrentGameState()			{return get()->m_currentState;}
		static void			setState(EGameState state)		{get()->setGameState(state);}
		static void			setSpeed(int speed)				{get()->impSetSpeed(speed);}
		static void			setCycle(int cycle)				{if(get()->m_cycle>=99999)get()->m_cycle=0;else get()->m_cycle=cycle;}
		static void			save(std::string savenm)		{get()->impSave(savenm);}

	private:

		CCluster*	m_cluster;
		CellGlyphs	m_cellStateDesc;

		int			m_speed=0;
		int			m_cycle=0;
		bool		m_bIsRunning=false;
		SConfig*	m_config;
		int			m_liveColor=0,m_deadColor=0;
		char		m_liveGlyph[2]{' ',' '};
		char		m_deadGlyph[2]{' ',' '};

	private:

		GameOfLife(){}

		void		setCellDisplayType(ECellState ecd,std::pair<char,char> chrs);
		void		rules();
		void		toggleGrid();
		void		destroy();

		void		impInit();
		void		impUpdate(unsigned long long dt);
		void		impOnInput(Event* event);
		void		impDraw(WINDOW* window);
		void		impSetSpeed(int speed);
		void		impReset();
		void		impLoad(std::string savename);
		void		impSave(std::string savename);

	private:

		static	GameOfLife*	m_instance;

	public:

		static	GameOfLife*	get(){return (m_instance)?m_instance:m_instance=new GameOfLife;}
};

inline GameOfLife*	GameOfLife::m_instance=nullptr;
