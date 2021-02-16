#pragma once

class GameData{

	public:
		
		GameData(GameData& other)=delete;
		void operator=(const GameData&)=delete;
		
		void					init();

		static int				getFactor()							{return get()->factor;}
		static int				getSize()							{return get()->size;}
		static int				getSpeed()							{return get()->speed;}

		static void				setFactor(int t_factor)				{get()->factor=t_factor;get()->size=t_factor*2;}
		static void				setSpeed(int t_speed)				{get()->speed=t_speed;}

	private:

		GameData(){}
		int						factor,
								size,
								speed;

		static GameData*		m_instance;
	
	public:
	
		static GameData*		get()								{return (m_instance)?m_instance:m_instance=new GameData;}
};

inline GameData* GameData::m_instance=nullptr;
