#include "VmpcBackupDeleteOldFilesScreen.hpp"

#include <lcdgui/screens/window/CancelOkScreen.hpp>

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
    
    // Set recommended actions. When the below are available,
    // default to YES.
    actionValues[0] = storeExists;
    actionValues[1] = keysExists;
    actionValues[2] = nvramExists;
    actionValues[3] = vmpcSpecificExists;
    
    // Never delete by default
    actionValues[4] = false;
    actionValues[5] = false;
}

void VmpcBackupDeleteOldFilesScreen::function(int i)
{
    init();

	switch (i)
    {
        case 3:
            openScreen("vmpc-clean");
            break;
        case 4:
            if (find(begin(actionAvailability), end(actionAvailability), true) == end(actionAvailability))
            {
                return;
            }
            
            // Delete old application
            if (actionValues[4])
            {
                auto cancelOkScreen = mpc.screens->get<CancelOkScreen>("vmpc-delete-old-vmpc-application");

                auto cancelAction = [&]() {
                    openScreen("vmpc-backup-delete-old-files");
                };
                
#ifdef _WIN32
                auto okAction = [&]() {
                    auto programFilesPath1 = "C:/Program Files/vMPC";
                    auto programFilesPath2 = "C:/Program Files (x86)/vMPC";
                    auto programFiles1 = make_shared<Directory>(programFilesPath1);
                    auto programFiles2 = make_shared<Directory>(programFilesPath2);

                    if (programFiles1->exists())
                        Directory::deleteRecursive(programFiles1);
                    if (programFiles2->exists())
                        Directory::deleteRecursive(programFiles2);

                    openScreen("vmpc-clean");
                };
#elif defined __APPLE__
                auto okAction = [&]() {
                    auto applicationPath = "/Applications/vMPC.app/";
                    auto application = make_shared<Directory>(applicationPath);
                    
                    if (application->exists())
                        Directory::deleteRecursive(application);
                    
                    openScreen("vmpc-clean");
                };
#endif
                cancelOkScreen->setCancelAction(cancelAction);
                cancelOkScreen->setOkAction(okAction);
                
                openScreen("vmpc-delete-old-vmpc-application");
            }
            
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
    auto hideDoIt = find(begin(actionValues), end(actionValues), true) == end(actionValues);
    
    findChild<FunctionKey>("fk4").lock()->Hide(hideDoIt);
    
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
