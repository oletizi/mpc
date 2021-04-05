#include "VmpcCleanScreen.hpp"

#include <sys/Home.hpp>
#include <file/Directory.hpp>
#include <file/FileUtil.hpp>
#include <thirdp/PlatformFolders/platform_folders.h>

using namespace mpc::lcdgui::screens;
using namespace moduru::file;
using namespace std;

VmpcCleanScreen::VmpcCleanScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "vmpc-clean", layerIndex)
{
    initOldFilesFound();
}

void VmpcCleanScreen::initOldFilesFound()
{
    auto home = moduru::sys::Home::get();
    auto sep = FileUtil::getSeparator();
    
    auto vmpcPath = home + sep + "vMPC";
    auto vmpcDirExists = Directory(vmpcPath).exists();
    
    auto programFilesPath1 = "C:/Program Files/vMPC";
    auto programFilesPath2 = "C:/Program Files (x86)/vMPC";
    auto applicationPath = "/Applications/vMPC.app";
    
    auto programFiles1Exists = Directory(programFilesPath1).exists();
    auto programFiles2Exists = Directory(programFilesPath2).exists();
    auto applicationExists = Directory(applicationPath).exists();
    
    oldFilesFound = vmpcDirExists || programFiles1Exists || programFiles2Exists || applicationExists;
}

void VmpcCleanScreen::open()
{
    findChild<Field>("show-on-startup").lock()->setAlignment(Alignment::Centered);

    initOldFilesFound();
    
    auto bgName = oldFilesFound ? "vmpc-clean" : "vmpc-clean-no-files";
    
    findChild<Background>("").lock()->setName(bgName);
    
    displayShowOnStartup();
}

void VmpcCleanScreen::function(int i)
{
    switch (i)
    {
        case 0:
            openScreen("vmpc-settings");
            break;
        case 1:
            openScreen("vmpc-keyboard");
            break;
        case 2:
            openScreen("vmpc-auto-save");
            break;
    }
}

void VmpcCleanScreen::turnWheel(int i)
{
    init();

	if (param.compare("show-on-startup") == 0)
	{
        if (!oldFilesFound)
            return;
        
        setShowOnStartup(i > 0);
	}
}

void VmpcCleanScreen::setShowOnStartup(bool b)
{
	showOnStartup = b;
	displayShowOnStartup();
}

void VmpcCleanScreen::displayShowOnStartup()
{
    auto label = findChild<Label>("show-on-startup").lock();
    auto field = findChild<Field>("show-on-startup").lock();
    
    label->Hide(!oldFilesFound);
    field->Hide(!oldFilesFound);
    
    if (!oldFilesFound)
        return;
    
    field->takeFocus("");
	field->setText(showOnStartup ? "YES" : "NO");
}

void VmpcCleanScreen::openWindow()
{
    if (!oldFilesFound)
        return;
    
    openScreen("vmpc-backup-delete-old-files");
}
