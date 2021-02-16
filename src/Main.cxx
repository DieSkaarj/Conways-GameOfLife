#include <sys/ioctl.h>

#include "CGame.h"
#include "NTime.h"

int main(int argc,char** argv){

	int width{0},height{0};
	struct winsize win;

	ioctl(fileno(stdout),TIOCGWINSZ,&win);
	width	=static_cast<int>(win.ws_col);
	height	=static_cast<int>(win.ws_row);

	GameData::setFactor(height);
	GameData::setSpeed(50);

	GameOfLife::init();

	unsigned long long time		{NTime::getTicks()};
	unsigned long long delta	{0};

	while(GameOfLife::isRunning()){

		delta=(NTime::getTicks()-time);
		time=(NTime::getTicks());

		CInput		::update(delta);
		GameOfLife	::update(delta);
		CScreen		::update();
	}

	CScreen::shutdown();

	return 0;
}
