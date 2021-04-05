#include "VmpcAutoSaveScreen.hpp"

using namespace mpc::lcdgui::screens;
using namespace std;

VmpcAutoSaveScreen::VmpcAutoSaveScreen(mpc::Mpc& mpc, const int layerIndex)
: ScreenComponent(mpc, "vmpc-auto-save", layerIndex)
{
}

void VmpcAutoSaveScreen::open()
{
    findChild<TextComp>("msg").lock()->setText("Only applies to standalone");

    displayAutoSaveOnExit();
    displayAutoLoadOnStart();
}

void VmpcAutoSaveScreen::function(int i)
{
    switch (i)
    {
        case 0:
            openScreen("vmpc-settings");
            break;
        case 1:
            openScreen("vmpc-keyboard");
            break;
        case 3:
            openScreen("vmpc-clean");
            break;
    }
}

void VmpcAutoSaveScreen::turnWheel(int i)
{
    init();
    
    if (param.compare("auto-save-on-exit") == 0)
    {
        setAutoSaveOnExit(autoSaveOnExit + i);
    }
    else if (param.compare("auto-load-on-start") == 0)
    {
        setAutoLoadOnStart(autoLoadOnStart + i);
    }
}

void VmpcAutoSaveScreen::setAutoSaveOnExit(int i)
{
    if (i < 0 || i > 2)
        return;
    
    autoSaveOnExit = i;
    displayAutoSaveOnExit();
}

void VmpcAutoSaveScreen::setAutoLoadOnStart(int i)
{
    if (i < 0 || i > 2)
        return;
    
    autoLoadOnStart = i;
    displayAutoLoadOnStart();
}

void VmpcAutoSaveScreen::displayAutoSaveOnExit()
{
    findField("auto-save-on-exit").lock()->setText(autoSaveOnExitNames[autoSaveOnExit]);
}

void VmpcAutoSaveScreen::displayAutoLoadOnStart()
{
    findField("auto-load-on-start").lock()->setText(autoLoadOnStartNames[autoLoadOnStart]);
}

int VmpcAutoSaveScreen::getAutoSaveOnExit()
{
    return autoSaveOnExit;
}

int VmpcAutoSaveScreen::getAutoLoadOnStart()
{
    return autoLoadOnStart;
}
