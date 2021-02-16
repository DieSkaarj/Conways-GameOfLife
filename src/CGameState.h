/************************************************************************/
/*																		*/
/*																		*/
/*	Class: CGameState													*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include <array>

#include "Enums.h"
#include "CState.h"

/*	GameState describes a pointer to a game state and a pair of *
 *	ints that are used to determine the colour of the cluster.	*/

typedef std::pair<CState*,std::pair<int,int>> GameState;
struct SStateInfo{

	CState*				state;
	std::pair<int,int>	colors;
	std::string 		name;
	EGameState			stateType;
};

typedef std::pair<int,int> ColorPair;

class CGameStates{

	public:

		CGameStates();
		~CGameStates(){destroy();}

	protected:

		void			init();
		void			update(unsigned long long dt);
		void			onInput(Event* event);

		void			setGameState(EGameState state);
		SStateInfo		constructGameState(EGameState type,CState* state,std::string name,std::pair<int,int> colors);

		ColorPair		getColors(EGameState state)	{return m_stateInfo[state].colors;}
		std::string		getStateTag(EGameState);

	protected:

		SStateInfo*				m_currentState;
		SStateInfo*				m_nextState;

	private:

		std::array<SStateInfo,5>m_stateInfo;

	private:

		void			destroy();
};
