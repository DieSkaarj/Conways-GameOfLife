/************************************************************************/
/*																		*/
/*																		*/
/*	Class:			CState												*/
/*	Description:	This is the pure base class that defines what a		*/
/*	a state is.															*/
/*																		*/
/************************************************************************/

#pragma once

struct Event;

class CState{

	public:

		virtual~CState(){}

		virtual void		enter(unsigned long long ticks)=0;
		virtual void		exit(unsigned long long ticks)=0;
		virtual	void		update(unsigned long long ticks)=0;
		virtual void		onInput(Event* event)=0;
};
