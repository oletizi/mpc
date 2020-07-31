#include "LoadScreen.hpp"

#include <audiomidi/AudioMidiServices.hpp>
#include <audiomidi/SoundPlayer.hpp>

#include <lcdgui/screens/window/DirectoryScreen.hpp>
#include <lcdgui/screens/window/LoadASequenceScreen.hpp>
#include <lcdgui/screens/dialog2/PopupScreen.hpp>

#include <disk/AbstractDisk.hpp>
#include <disk/MpcFile.hpp>

#include <file/File.hpp>
#include <file/FileUtil.hpp>

using namespace mpc::lcdgui::screens;
using namespace mpc::lcdgui::screens::window;
using namespace mpc::lcdgui::screens::dialog2;
using namespace mpc::sampler;
using namespace moduru::lang;
using namespace moduru::file;
using namespace std;

LoadScreen::LoadScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "load", layerIndex)
{
}

void LoadScreen::open()
{
	displayView();

	displayDirectory();
	displayFile();
	displaySize();

	displayFreeSnd();
	findLabel("freeseq").lock()->setText("  2640K");

	auto splitFileName = StrUtil::split(getSelectedFileName(), '.');
	auto playable = splitFileName.size() > 1 && (StrUtil::eqIgnoreCase(splitFileName[1], "snd") || StrUtil::eqIgnoreCase(splitFileName[1], "wav"));
	ls.lock()->setFunctionKeysArrangement(playable ? 1 : 0);
}

void LoadScreen::function(int i)
{
	init();
	
	auto disk = mpc.getDisk().lock();

	switch (i)
	{
	case 1:
		ls.lock()->openScreen("save");
		break;
	case 2:
		ls.lock()->openScreen("format");
		break;
	case 3:
		//ls.lock()->openScreen("setup");
		break;
	case 4:
	{
		auto controls = mpc.getControls().lock();

		if (controls->isF5Pressed())
		{
			return;
		}

		controls->setF5Pressed(true);

		auto file = getSelectedFile();

		if (!file->isDirectory())
		{
			
			bool started = mpc.getAudioMidiServices().lock()->getSoundPlayer().lock()->start(file->getFile().lock()->getPath());
			
			auto name = file->getFsNode().lock()->getNameWithoutExtension();

			ls.lock()->openScreen("popup");
			auto popupScreen = dynamic_pointer_cast<PopupScreen>(mpc.screens->getScreenComponent("popup"));

			if (started)
			{
				popupScreen->setText("Playing " + name);
			}
			else
			{
				popupScreen->setText("Can't play " + name);
			}
		}
		break;
	}
	case 5:
	{
		if (!disk || disk->getFileNames().size() == 0)
		{
			return;
		}
		
		auto selectedFile = getSelectedFile();
		auto ext = moduru::file::FileUtil::splitName(selectedFile->getName())[1];
		
		if (StrUtil::eqIgnoreCase(ext, "snd") || StrUtil::eqIgnoreCase(ext, "wav"))
		{
			mpc.loadSound(false);
			return;
		}
		
		if (StrUtil::eqIgnoreCase(ext, "pgm"))
		{
			ls.lock()->openScreen("load-a-program");
			return;
		}
		
		if (StrUtil::eqIgnoreCase(ext, "mid"))
		{
			ls.lock()->openScreen("load-a-sequence");
			return;
		}
		
		if (StrUtil::eqIgnoreCase(ext, "all"))
		{
			ls.lock()->openScreen("mpc2000xl-all-file");
			return;
		}
		
		if (StrUtil::eqIgnoreCase(ext, "aps"))
		{
			ls.lock()->openScreen("load-aps-file");
			return;
		}

		if (isSelectedFileDirectory())
		{
			if (disk->moveForward(getSelectedFile()->getName()))
			{
				mpc.getDisk().lock()->initFiles();

				fileLoad = 0;

				displayView();
				displayDirectory();
				displayFile();
				displaySize();

				auto splitFileName = StrUtil::split(getSelectedFileName(), '.');
				auto playable = splitFileName.size() > 1 && (StrUtil::eqIgnoreCase(splitFileName[1], "snd") || StrUtil::eqIgnoreCase(splitFileName[1], "wav"));
				ls.lock()->setFunctionKeysArrangement(playable ? 1 : 0);
			}
		}
		break;
	}
	}
}

void LoadScreen::openWindow()
{
	init();
	auto disk = mpc.getDisk().lock();

	if (param.compare("directory") == 0 || param.compare("file") == 0)
	{
		if (!disk)
		{
			return;
		}

		auto directoryScreen = dynamic_pointer_cast<DirectoryScreen>(mpc.screens->getScreenComponent("directory"));
		directoryScreen->previousScreenName = "load";
		directoryScreen->findYOffset0();
		directoryScreen->setYOffset1(fileLoad);
		ls.lock()->openScreen("directory");
		return;
	}
}

void LoadScreen::turnWheel(int i)
{
	init();

	if (param.compare("view") == 0)
	{
		setView(view + i);
	}
	else if (param.compare("file") == 0)
	{
		setFileLoadWithMaxCheck(fileLoad + i);
	}

	auto splitFileName = StrUtil::split(getSelectedFileName(), '.');
	auto playable = splitFileName.size() > 1 && (StrUtil::eqIgnoreCase(splitFileName[1], "snd") || StrUtil::eqIgnoreCase(splitFileName[1], "wav"));
	ls.lock()->setFunctionKeysArrangement(playable ? 1 : 0);
}

void LoadScreen::displayView()
{
	findField("view").lock()->setText(views[view]);
}

void LoadScreen::displayDirectory()
{
	findLabel("directory").lock()->setText(u8"\u00C2" + mpc.getDisk().lock()->getDirectoryName());
}

void LoadScreen::displayFreeSnd()
{
	findLabel("freesnd").lock()->setText(" " + StrUtil::padLeft(to_string((int)(mpc.getSampler().lock()->getFreeSampleSpace() * 0.001)), " ", 5) + "K");
}

void LoadScreen::displayFile()
{
	if (mpc.getDisk().lock()->getFileNames().size() == 0)
	{
		findField("file").lock()->setText("");
		return;
	}

	auto selectedFileName = getSelectedFileName();	
	auto selectedFile = getSelectedFile();
	
	if (selectedFileName.length() != 0 && selectedFile != nullptr && selectedFile->isDirectory())
	{
		findField("file").lock()->setText(u8"\u00C3" + StrUtil::padRight(FileUtil::splitName(selectedFileName)[0], " ", 16));
	}
	else
	{
		auto periodIndex = selectedFileName.find_last_of(".");

		if (periodIndex != string::npos)
		{
			auto extension = selectedFileName.substr(periodIndex, selectedFileName.length());
			auto fileName = StrUtil::padRight(selectedFileName.substr(0, periodIndex), " ", 16);
			selectedFileName = fileName + extension;
		}

		findField("file").lock()->setText(selectedFileName);
	}
}

int LoadScreen::getFileSize()
{
	auto disk = mpc.getDisk().lock();
	
	if (disk->getFile(fileLoad) == nullptr || disk->getFile(fileLoad)->isDirectory())
	{
		return 0;
	}
	
	return (int) ceil(disk->getFile(fileLoad)->length() / 1024.0);
}

void LoadScreen::displaySize()
{
	if (mpc.getDisk().lock()->getFileNames().size() == 0)
	{
		findLabel("size").lock()->setText("      K");
		return;
	}
	
	findLabel("size").lock()->setText(StrUtil::padLeft(to_string(getFileSize()), " ", 6) + "K");
}

void LoadScreen::setView(int i)
{
	if (i < 0 || i > 8)
	{
		return;
	}
	
	view = i;
	
	mpc.getDisk().lock()->initFiles();
	
	fileLoad = 0;

	displayView();
	displayDirectory();
	displayFile();
	displaySize();
}


mpc::disk::MpcFile* LoadScreen::getSelectedFile()
{
	return mpc.getDisk().lock()->getFile(fileLoad);
}

string LoadScreen::getSelectedFileName()
{
	auto fileNames = mpc.getDisk().lock()->getFileNames();
	
	if (fileNames.size() <= fileLoad)
	{
		return "";
	}

	return fileNames[fileLoad];
}

bool LoadScreen::isSelectedFileDirectory()
{
	return mpc.getDisk().lock()->getFile(fileLoad)->isDirectory();
}

void LoadScreen::setFileLoadWithMaxCheck(int i)
{
	if (i >= mpc.getDisk().lock()->getFileNames().size())
	{
		return;
	}

	setFileLoad(i);
}

void LoadScreen::setFileLoad(int i)
{
	if (i < 0)
	{
		return;
	}

	fileLoad = i;
	displayFile();
	displaySize();
}
