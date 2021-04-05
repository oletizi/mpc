#include "VmpcBackupDeleteOldFilesScreen.hpp"

using namespace mpc::lcdgui::screens::window;
using namespace std;

VmpcBackupDeleteOldFilesScreen::VmpcBackupDeleteOldFilesScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "vmpc-backup-delete-old-files", layerIndex)
{
}

void VmpcBackupDeleteOldFilesScreen::function(int i)
{
    init();

	switch (i)
    {
    case 3:
        openScreen("vmpc-clean");
        break;
    }
}

void VmpcBackupDeleteOldFilesScreen::turnWheel(int)
{
    
}

void VmpcBackupDeleteOldFilesScreen::up()
{
    init();
    
    auto yPos = stoi(param.substr(6));
    
    if (yPos > 0)
    {
        ScreenComponent::up();
        return;
    }
}

void VmpcBackupDeleteOldFilesScreen::down()
{
    init();
    
    auto yPos = stoi(param.substr(6));
    
    if (yPos < 3)
    {
        ScreenComponent::down();
        return;
    }
}
