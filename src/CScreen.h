/************************************************************************/
/*																		*/
/*																		*/
/*	Class: Screen														*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include <array>
#include <vector>

#include <form.h>
#include <panel.h>

#include "CMenu.h"
#include "CData.h"
#include "Enums.h"
#include "SWindows.h"

#define MSG_BORDER_X_SPACING 4
#define MSG_BORDER_Y_SPACING 4
#define SIDEPANE_LENGTH 16

#define CR_WHITE_GREEN COLOR_PAIR(1)
#define CR_YELLOW_CYAN COLOR_PAIR(2)
#define CR_WHITE_RED COLOR_PAIR(3)
#define CR_BLACK_WHITE COLOR_PAIR(4)
#define CR_WHITE_BLACK COLOR_PAIR(5)
#define CR_GREEN_RED COLOR_PAIR(6)
#define CR_RED_WHITE COLOR_PAIR(7)
#define CR_BLUE_WHITE COLOR_PAIR(8)
#define CR_WHITE_BLUE COLOR_PAIR(9)
#define CR_BLUE_WHITE2 COLOR_PAIR(10)

class CScreen{

	public:

		~CScreen(){}

		class GUI{

			public:

				~GUI(){destroy();}

				static void			promptMessage(std::vector<std::string> msg)	{get()->impDisplayMsg(msg);}
				static SFrame*		getMessageFrame()							{return get()->m_frames[FRAME_MESSAGES];}

				void	init();
				void	draw();
				void	onInput(Event* e);

				void	destroy();

			public:

				bool	m_bHasFocus=true;
				std::array<SFrame*,5>	m_frames={new SFrame,new SFrame,new SFrame,new SFrame,new SFrame};

			private:

				GUI(){init();}

				void	makeMenu(int h,int w,int y,int x);
				void	makeDish(int h,int w,int y,int x);
				void	makeInfo(int h,int w,int y,int x);

				void	makeCursor();

				void	impDisplayMsg(std::vector<std::string> msg);

			private:

				CMenu*	menu;
				CData*	data;

				int		m_sidePanelWidth=16;

				static GUI*	m_instance;

			public:

				static	GUI*		get(){return(m_instance)?m_instance:m_instance=new GUI;}
		};

	public:


		static void				onResize()								{get()->impOnResize();}

		static void				init()									{get()->impInit();}
		static void				update()								{get()->impUpdate();}
		static void				shutdown()								{get()->impShutdown();}
		static void				onInput(Event* e)						{get()->gui->onInput(e);}

		static int				getWidth()								{return get()->m_width;}
		static int				getHeight()								{return get()->m_height;}
		static void				destroy(SFrame* frm);
		static SFrame*			getFrame(EFrameType eFrameId)			{return get()->gui->m_frames[eFrameId];}

		static void				setMenuFocus(bool x)					{get()->gui->m_bHasFocus=x;}

	private:

		static GUI*				gui;
		static CScreen*			m_instance;

		int 					m_width,
								m_height;
	
	public:

		static	CScreen*		get(){return(m_instance)?m_instance:m_instance=new CScreen;}

	private:

		CScreen(){}

		void					impInit();
		void					impUpdate();
		void					impShutdown();
		void					impOnResize();
};

inline CScreen* CScreen::m_instance=nullptr;
inline CScreen::GUI* 	CScreen::GUI::m_instance=nullptr;
inline CScreen::GUI* 	CScreen::gui=nullptr;
