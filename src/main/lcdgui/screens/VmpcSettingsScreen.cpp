#include "VmpcSettingsScreen.hpp"

using namespace mpc::lcdgui::screens;
using namespace std;

VmpcSettingsScreen::VmpcSettingsScreen(const int layerIndex)
	: ScreenComponent("vmpc-settings", layerIndex)
{
}

void VmpcSettingsScreen::open()
{
	displayInitialPadMapping();
}

void VmpcSettingsScreen::turnWheel(int i)
{
    init();

	if (param.compare("initial-pad-mapping") == 0)
	{
		setInitialPadMapping(initialPadMapping + i);
	}
}

void VmpcSettingsScreen::setInitialPadMapping(int i)
{
	if (i < 0 || i > 1)
	{
		return;
	}

	initialPadMapping = i;

	displayInitialPadMapping();
}

void VmpcSettingsScreen::displayInitialPadMapping()
{
	findField("initial-pad-mapping").lock()->setText(initialPadMappingNames[initialPadMapping]);
}
