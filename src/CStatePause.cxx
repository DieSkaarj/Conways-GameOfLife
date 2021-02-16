#include "CGame.h"
#include "CCluster.h"
#include "CStatePause.h"

void CStatePause::enter(unsigned long long t_dt){

}

void CStatePause::update(unsigned long long t_dt){

	/*
	 *	Only evolve the cluster if User steps. 
	 */

	if(m_step){

		GameOfLife::getCluster()->evolve();
		GameOfLife::setCycle(GameOfLife::getCycle()+1);
		m_step=false;
	}
}

void CStatePause::exit(unsigned long long t_dt){

}

void CStatePause::onInput(Event* e){

	if(e->key==32) m_step=true;
}
