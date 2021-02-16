#include "NGCmd.h"
#include "CCluster.h"
#include "CStatePlay.h"

void CStatePlay::enter(unsigned long long t_dt){

	/*
	 *	Reset timer.
	 */

	timerA=timerB=0;
}

void CStatePlay::exit(unsigned long long t_dt){

}

void CStatePlay::update(unsigned long long t_dt){

	CCluster* cluster	=GameOfLife::getCluster();
	int speed			=GameData::getSpeed();	

	/*
	 *	The frequency of evolution is calculated as fraction of 1 then
	 *	multiplied by a factor of 10000 to adjust to ticks.
	 */

	timerA=((1.f/speed)*10000);
	timerB+=t_dt;

	if(timerB>=timerA)
	{
		cluster->evolve();
		timerB-=timerA;
		GameOfLife::setCycle(GameOfLife::getCycle()+1);
	}
}

void CStatePlay::onInput(Event* e){

	if(e->key==32) GCmd::set_mode_pause();
}
