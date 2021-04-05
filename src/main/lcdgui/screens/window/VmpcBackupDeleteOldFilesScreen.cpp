#include "VmpcBackupDeleteOldFilesScreen.hpp"

#include <sys/Home.hpp>
#include <file/FileUtil.hpp>
#include <file/Directory.hpp>
#include <file/File.hpp>

using namespace mpc::lcdgui::screens::window;
using namespace moduru::file;
using namespace std;

VmpcBackupDeleteOldFilesScreen::VmpcBackupDeleteOldFilesScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "vmpc-backup-delete-old-files", layerIndex)
{
}

void VmpcBackupDeleteOldFilesScreen::open()
{
    for (int i = 0; i < 4; i++)
    {
        auto field = findChild<Field>("action" + to_string(i)).lock();
        field->setAlignment(Alignment::Centered);
    }
    
    determineAvailableActions();
    displayActions();
}

void VmpcBackupDeleteOldFilesScreen::determineAvailableActions()
{
    auto home = moduru::sys::Home::get();
    auto sep = FileUtil::getSeparator();
    
    auto vmpcPath = home + sep + "vMPC" + sep;
    auto vmpcDirExists = Directory(vmpcPath).exists();
    
    bool storeExists = false;
    bool keysExists = false;
    bool nvramExists = false;
    bool vmpcSpecificExists = false;
    
    if (vmpcDirExists)
    {
        auto storePath = vmpcPath + "Stores" + sep + "MPC2000XL";
        storeExists = File(storePath, 0).exists();
        
        auto keysPath = vmpcPath + "resources" + sep + "keys.txt";
        keysExists = File(keysPath, 0).exists();
        
        auto nvramPath = vmpcPath + "resources" + sep + "nvram.vmp";
        nvramExists = File(nvramPath, 0).exists();
        
        auto vmpcSpecificPath = vmpcPath + "resources" + sep + "vmpc-specific.ini";
        vmpcSpecificExists = File(vmpcSpecificPath, 0).exists();
    }
    
    auto programFilesPath1 = "C:/Program Files/vMPC";
    auto programFilesPath2 = "C:/Program Files (x86)/vMPC";
    auto applicationPath = "/Applications/vMPC.app";
    
    auto programFiles1Exists = Directory(programFilesPath1).exists();
    auto programFiles2Exists = Directory(programFilesPath2).exists();
    auto applicationExists = Directory(applicationPath).exists();
    
    // Copy to new locations
    actionAvailability[0] = storeExists;
    actionAvailability[1] = keysExists;
    actionAvailability[2] = nvramExists;
    actionAvailability[3] = vmpcSpecificExists;
    
    // Delete permanently
    actionAvailability[4] = programFiles1Exists || programFiles2Exists || applicationExists;
    actionAvailability[5] = vmpcDirExists;
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

void VmpcBackupDeleteOldFilesScreen::turnWheel(int i)
{
    init();
    
    auto yPos = stoi(param.substr(6));
    
    if (!actionAvailability[yPos + yOffset])
        return;
    
    actionValues[yPos + yOffset] = i > 0;
    displayActions();
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
    
    if (yOffset > 0)
    {
        yOffset--;
        displayActions();
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
    
    if (yOffset + 4 + 1 <= actionNames.size())
    {
        yOffset++;
        displayActions();
    }
}

void VmpcBackupDeleteOldFilesScreen::displayActions()
{
    if (yOffset == 0)
        findBackground().lock()->setName("vmpc-backup-delete-old-files-down");
    else if (yOffset > 0 && yOffset < actionNames.size() - 4)
        findBackground().lock()->setName("vmpc-backup-delete-old-files");
    else
        findBackground().lock()->setName("vmpc-backup-delete-old-files-up");
    
    for (int i = 0; i < 4; i++)
    {
        auto label = findChild<Label>("action" + to_string(i)).lock();
        auto field = findChild<Field>("action" + to_string(i)).lock();
        
        label->setText(actionNames[i + yOffset]);
        
        if (actionAvailability[i + yOffset])
            field->setText(actionValues[i + yOffset] ? "YES" : "NO");
        else
            field->setText("--");
    }
}
