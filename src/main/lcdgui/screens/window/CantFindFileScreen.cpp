#include "CantFindFileScreen.hpp"

using namespace mpc::lcdgui::screens::window;
using namespace std;

CantFindFileScreen::CantFindFileScreen(const int layerIndex) 
	: ScreenComponent("cant-find-file", layerIndex)
{
}

void CantFindFileScreen::open()
{
	findField("file").lock()->setText(fileName);
}

void CantFindFileScreen::function(int i)
{
	init();

	switch (i)
	{
	case 1:
		skipAll = true;
		waitingForUser = false;
		break;
	case 2:
		waitingForUser = false;
		break;
	}
}
