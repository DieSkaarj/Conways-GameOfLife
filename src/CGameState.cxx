#include "CGameState.h"
#include "CStatePlay.h"
#include "CStatePaint.h"
#include "CStateSave.h"
#include "CStateLoad.h"
#include "CStatePause.h"

CGameStates::CGameStates(){

	init();
}

void CGameStates::init(){

	m_stateInfo[MODE_PLAY] =constructGameState(MODE_PLAY, new CStatePlay(this), "Running",std::pair(1,2));
	m_stateInfo[MODE_PAUSE]=constructGameState(MODE_PAUSE,new CStatePause(this),"Paused", std::pair(5,7));
	m_stateInfo[MODE_PAINT]=constructGameState(MODE_PAINT,new CStatePaint(this),"Create", std::pair(3,2));
	m_stateInfo[MODE_LOAD] =constructGameState(MODE_LOAD, new CStateLoad(this), "Load",   std::pair(5,7));
	m_stateInfo[MODE_SAVE] =constructGameState(MODE_SAVE, new CStateSave(this), "Save",   std::pair(5,7));

	m_currentState=&m_stateInfo[MODE_PLAY];
	setGameState(MODE_PAUSE);
}

SStateInfo CGameStates::constructGameState(EGameState t_type,CState* t_state,std::string t_name,std::pair<int,int> t_colors){

	SStateInfo* stateInf=new SStateInfo;

	stateInf->state		=t_state;
	stateInf->colors	=t_colors;
	stateInf->name		=t_name;
	stateInf->stateType	=t_type;
	
	return *stateInf;
}

void CGameStates::update(unsigned long long t_dt){

	if(m_currentState==nullptr){

		m_currentState=m_nextState;
		m_currentState->state->enter(t_dt);
	}else if(m_currentState==m_nextState){

		m_currentState->state->update(t_dt);
	}else{

		m_currentState->state->exit(t_dt);
		m_currentState=m_nextState;
		m_currentState->state->enter(t_dt);
	}
}

void CGameStates::onInput(Event* e){

	m_currentState->state->onInput(e);
}

void CGameStates::setGameState(EGameState t_state){

	switch(t_state){

		case MODE_PLAY: 	m_nextState=&m_stateInfo[MODE_PLAY];	break;
		case MODE_PAINT:	m_nextState=&m_stateInfo[MODE_PAINT];break;
		case MODE_PAUSE:	m_nextState=&m_stateInfo[MODE_PAUSE];break;
		case MODE_LOAD:		m_nextState=&m_stateInfo[MODE_LOAD];	break;
		case MODE_SAVE:		m_nextState=&m_stateInfo[MODE_SAVE];	break;
	};
}

void CGameStates::destroy(){

	m_nextState=nullptr;
	delete m_nextState;
	m_currentState=nullptr;
	delete m_currentState;
}
