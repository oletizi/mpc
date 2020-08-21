#include "FileAlreadyExistsScreen.hpp"

#include <Util.hpp>

#include <disk/AbstractDisk.hpp>
#include <disk/MpcFile.hpp>

#include <file/all/AllParser.hpp>

#include <lcdgui/Screens.hpp>
#include <lcdgui/screens/window/SaveASoundScreen.hpp>
#include <lcdgui/screens/window/NameScreen.hpp>

using namespace mpc::lcdgui;
using namespace mpc::lcdgui::screens::dialog;
using namespace mpc::lcdgui::screens::window;
using namespace std;

FileAlreadyExistsScreen::FileAlreadyExistsScreen(mpc::Mpc& mpc, const int layerIndex) 
	: ScreenComponent(mpc, "file-already-exists", layerIndex)
{
}

void FileAlreadyExistsScreen::function(int i)
{
	ScreenComponent::function(i);
		
	switch (i)
	{
	case 2:
	{
		auto disk = mpc.getDisk().lock();
		auto nameScreen = dynamic_pointer_cast<NameScreen>(mpc.screens->getScreenComponent("name"));

		if (ls.lock()->getPreviousScreenName().compare("save-a-program") == 0)
		{
			auto pfileName = mpc::Util::getFileName(nameScreen->getName()) + ".PGM";
			auto success = disk->getFile(pfileName)->del();

			if (success)
			{
				disk->flush();
				disk->initFiles();
				disk->writeProgram(program.lock().get(), pfileName);
			}
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-a-sequence") == 0)
		{
			auto sfileName = mpc::Util::getFileName(nameScreen->getName()) + ".MID";
			auto success = disk->getFile(sfileName)->del();

			if (success)
			{
				disk->flush();
				disk->initFiles();
				disk->writeSequence(sequencer.lock()->getActiveSequence().lock().get(), sfileName);
				openScreen("save");
			}
			openScreen("save");
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-aps-file") == 0)
		{
			auto apsName = mpc::Util::getFileName(nameScreen->getName()) + ".APS";
			auto success = disk->getFile(apsName)->del();
			
			if (success)
			{
				disk->flush();
				disk->initFiles();
				apsSaver = make_unique<mpc::disk::ApsSaver>(mpc, apsName);
			}
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-all-file") == 0)
		{
			auto allName = mpc::Util::getFileName(nameScreen->getName()) + ".ALL";
			disk->initFiles();
			auto success = disk->getFile(allName)->del();
			
			if (success)
			{
				disk->flush();
				disk->initFiles();
				auto allParser = mpc::file::all::AllParser(mpc, mpc::Util::getFileName(nameScreen->getName()));
				auto f = disk->newFile(allName);
				auto bytes = allParser.getBytes();
				f->setFileData(&bytes);
				disk->flush();
				disk->initFiles();
				openScreen("save");
			}
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-a-sound") == 0)
		{
			auto s = sampler.lock()->getSound().lock();

			auto saveASoundScreen = dynamic_pointer_cast<SaveASoundScreen>(mpc.screens->getScreenComponent("save-a-sound"));

			auto type = saveASoundScreen->fileType;

			auto ext = string(type == 0 ? ".SND" : ".WAV");
			auto fileName = mpc::Util::getFileName(nameScreen->getName()) + ext;

			disk->getFile(fileName)->del();
			disk->flush();
			disk->initFiles();

			auto f = disk->newFile(fileName);

			if (type == 0)
			{
				disk->writeSound(s.get(), f);
			}
			else
			{
				disk->writeWav(s.get(), f);
			}

			disk->flush();
			disk->initFiles();
			openScreen("save");
		}
		break;
	}
	case 4:
	{
		auto nameScreen = dynamic_pointer_cast<NameScreen>(mpc.screens->getScreenComponent("name"));

		if (ls.lock()->getPreviousScreenName().compare("save-a-program") == 0)
		{
			nameScreen->parameterName ="savingpgm";
			openScreen("name");
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-a-sequence") == 0)
		{
			nameScreen->parameterName ="savingmid";
			openScreen("name");
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-aps-file") == 0)
		{
			nameScreen->parameterName ="savingaps";
			openScreen("name");
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-all-file") == 0)
		{
			nameScreen->parameterName ="save-all-file";
			openScreen("name");
		}
		else if (ls.lock()->getPreviousScreenName().compare("save-a-sound") == 0)
		{
			nameScreen->parameterName ="save-a-sound";
			openScreen("name");
		}
		break;
	}
	}
}
