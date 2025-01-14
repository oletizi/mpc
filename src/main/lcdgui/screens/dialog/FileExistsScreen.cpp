#include "FileExistsScreen.hpp"

#include <Util.hpp>

#include <disk/AbstractDisk.hpp>
#include <disk/MpcFile.hpp>

#include <file/all/AllParser.hpp>

#include <lcdgui/Screens.hpp>
#include <lcdgui/screens/window/SaveASoundScreen.hpp>
#include <lcdgui/screens/window/SaveAllFileScreen.hpp>
#include <lcdgui/screens/window/SaveApsFileScreen.hpp>
#include <lcdgui/screens/window/NameScreen.hpp>
#include <lcdgui/screens/dialog2/PopupScreen.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::lcdgui::screens::dialog;
using namespace mpc::lcdgui;
using namespace mpc::lcdgui::screens::dialog2;
using namespace mpc::lcdgui::screens::window;

using namespace moduru::lang;

using namespace std;

FileExistsScreen::FileExistsScreen(mpc::Mpc& mpc, const int layerIndex) 
	: ScreenComponent(mpc, "file-exists", layerIndex)
{
}

void FileExistsScreen::function(int i)
{
	ScreenComponent::function(i);
		
	switch (i)
	{
	case 2:
	{
		auto disk = mpc.getDisk().lock();
		auto nameScreen = mpc.screens->get<NameScreen>("name");

		if (ls.lock()->getPreviousScreenName().compare("save-a-program") == 0)
		{
			auto pfileName = mpc::Util::getFileName(nameScreen->getNameWithoutSpaces()) + ".PGM";
			auto success = disk->getFile(pfileName)->del();

			if (success)
			{
				disk->flush();
				disk->initFiles();
				disk->writePgm(program.lock(), pfileName);
			}
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-a-sequence") == 0)
		{
			auto sfileName = mpc::Util::getFileName(nameScreen->getNameWithoutSpaces()) + ".MID";
			auto success = disk->getFile(sfileName)->del();

			if (success)
			{
				disk->flush();
				disk->initFiles();
				disk->writeMid(sequencer.lock()->getActiveSequence().lock(), sfileName);
				openScreen("save");
			}
			openScreen("save");
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-aps-file") == 0)
		{
			auto saveApsFileScreen = mpc.screens->get<SaveApsFileScreen>("save-aps-file");
			auto apsFileName = saveApsFileScreen->fileName + ".APS";

			auto success = disk->getFile(apsFileName)->del();
			
			if (success)
			{
				disk->flush();
				disk->initFiles();
                disk->writeAps(apsFileName);
			}
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-all-file") == 0)
		{
            auto saveAllFileScreen = mpc.screens->get<SaveAllFileScreen>("save-all-file");
			auto allFileName = saveAllFileScreen->fileName + ".ALL";
			disk->initFiles();
			auto success = disk->getFile(allFileName)->del();
			
			if (success)
			{
				disk->flush();
				disk->initFiles();

                disk->writeAll(allFileName);
			}
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-a-sound") == 0)
		{
			auto s = sampler.lock()->getSound().lock();

			auto saveASoundScreen = mpc.screens->get<SaveASoundScreen>("save-a-sound");

			auto type = saveASoundScreen->fileType;

			auto ext = string(type == 0 ? ".SND" : ".WAV");
			auto fileName = mpc::Util::getFileName(nameScreen->getNameWithoutSpaces()) + ext;

			disk->getFile(fileName)->del();
			disk->flush();
			disk->initFiles();

			if (type == 0)
				disk->writeSnd(s, fileName);
			else
				disk->writeWav(s, fileName);

			openScreen("save");
		}
		break;
	}
	case 4:
	{
		auto nameScreen = mpc.screens->get<NameScreen>("name");

		vector<string> screens{ "save-a-program", "save-a-sequence", "save-a-sound" };

		auto previousScreen = ls.lock()->getPreviousScreenName();
        
        if (previousScreen.compare("save-aps-file") == 0)
        {
            const auto renamer = [&](const string& newName) {
                const auto apsFileName = newName + ".APS";
                
                auto disk = mpc.getDisk().lock();

                if (disk->checkExists(apsFileName))
                {
                    openScreen("file-exists");
                    mpc.getLayeredScreen().lock()->setPreviousScreenName("save-aps-file");
                    return;
                }
                
                disk->writeAps(mpc::Util::getFileName(apsFileName));
            };
            
            nameScreen->setRenamerAndScreenToReturnTo(renamer, "");
            openScreen("name");
        }
        else if (previousScreen.compare("save-all-file") == 0)
        {
            openScreen(previousScreen);
        }
		else if (find(begin(screens), end(screens), nameScreen->parameterName) != end(screens))
        {
            openScreen("name");
        }
		else if (find(begin(screens), end(screens), previousScreen) != end(screens))
		{
			nameScreen->parameterName = previousScreen;
			openScreen("name");
		}

		break;
	}
	}
}
