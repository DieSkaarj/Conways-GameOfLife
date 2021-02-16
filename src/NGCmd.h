#pragma once

#include <dirent.h>

#include "CGame.h"

namespace GCmd{

	static void set_mode_pattern(){
	
		GameOfLife::setState(MODE_PAINT);
	};
	
	static void set_mode_run(){
	
		GameOfLife::setState(MODE_PLAY);
	};
	
	static void set_mode_pause(){
	
		GameOfLife::setState(MODE_PAUSE);
	};
	
	static void reset_cluster(){

		FORM*	form;
		FIELD*	field[3];
		WINDOW*	win_form,*main_msg;
		std::string	label="Sq. Factor:";

		int H=5,
			W=(label.length()+4)+8,
			Y=((LINES+H)*.5)-H,
			X=((COLS+W)*.5)-W;

		main_msg=newwin(H,W,Y,X);
		wbkgd(main_msg,COLOR_PAIR(9));

		H-=2;
		W-=2;

		win_form=derwin(main_msg,H,W,1,1);
		box(main_msg,0,0);

		X=((W-6+label.length())*.5)-label.length();
		field[0]=new_field(1,label.length(),0,X,0,0);
		field[1]=new_field(1,3,0,(X+label.length()+1),0,0);
		field[2]=nullptr;

		set_field_fore(field[0],COLOR_PAIR(9));		
		set_field_fore(field[1],COLOR_PAIR(9));		
		set_field_back(field[1],COLOR_PAIR(9)|A_UNDERLINE);
		
		set_field_buffer(field[0],0,label.c_str());
		set_field_opts(field[0],O_VISIBLE|O_PUBLIC|O_AUTOSKIP);

		set_field_opts(field[1],O_VISIBLE|O_PUBLIC|O_EDIT|O_PASSOK|O_ACTIVE|O_STATIC|O_AUTOSKIP);
		set_field_type(field[1],TYPE_INTEGER,0,1,LINES);

		form=new_form(field);

		set_form_win(form,win_form);
		set_form_sub(form,derwin(win_form,getmaxy(win_form)-2,getmaxx(win_form)-2,1,1));
		post_form(form);

		set_current_field(form,field[1]);

		int 		newFactor=0;

		while((CInput::get()->e.key=wgetch(main_msg))!=10){

			switch(CInput::getKey()){

				case KEY_UP:	form_driver(form,REQ_BEG_FIELD);	break;
				case KEY_DOWN:	form_driver(form,REQ_END_FIELD);	break;
				case KEY_LEFT:	form_driver(form,REQ_PREV_CHAR);	break;
				case KEY_RIGHT:	form_driver(form,REQ_NEXT_CHAR);	break;
				case KEY_BACKSPACE:
				case 127:		form_driver(form,REQ_DEL_PREV);		break;
				case KEY_DC:	form_driver(form,REQ_DEL_CHAR);		break;
				default:		form_driver(form,CInput::getKey());	break;
			};

			wnoutrefresh(win_form);
		};

		form_driver(form,REQ_VALIDATION);

		std::string str=field_buffer(field[1],0);

		if(str!="   ")
			newFactor=std::stoi(field_buffer(field[1],0));
		else
			newFactor=GameData::getFactor();

		if(newFactor>LINES||newFactor<1) newFactor=LINES;

		GameData::setFactor(newFactor);
		GameOfLife::reset();

		unpost_form(form);
		free_form(form);
		free_field(field[0]);
		free_field(field[1]);
		delwin(win_form);
		form=nullptr;
		field[0]=nullptr;
		field[1]=nullptr;
		win_form=nullptr;
		delete form;
		delete field[0];
		delete field[1];
		delete win_form;
	};
	
	static void shutdown(){
	
		GameOfLife::shutdown();
	};
	
	static void	set_speed_up_1(){
	
		GameOfLife::setSpeed(GameOfLife::getSpeed()+1);
	};
	
	static void	set_speed_dwn_1(){
	
		GameOfLife::setSpeed(GameOfLife::getSpeed()-1);
	};
	
	static void	set_speed_up_10(){
	
		GameOfLife::setSpeed(GameOfLife::getSpeed()+10);
	};
	
	static void	set_speed_dwn_10(){
	
		GameOfLife::setSpeed(GameOfLife::getSpeed()-10);
	};
	
	static void	set_speed_0(){
	
		GameOfLife::setSpeed(0);
	};
	
	static void	set_speed_100(){
	
		GameOfLife::setSpeed(100);
	};
	
	static void save(){

		GameOfLife::setState(MODE_SAVE);
	};
	
	static void load(){

		GameOfLife::setState(MODE_LOAD);
	};
}
