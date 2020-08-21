#include "Mpc2000XlAllFileScreen.hpp"

#include <lcdgui/screens/LoadScreen.hpp>
#include <lcdgui/screens/window/LoadASequenceFromAllScreen.hpp>

#include <disk/AllLoader.hpp>

using namespace mpc::lcdgui::screens::window;
using namespace mpc::lcdgui::screens;
using namespace std;

Mpc2000XlAllFileScreen::Mpc2000XlAllFileScreen(mpc::Mpc& mpc, const int layerIndex) 
	: ScreenComponent(mpc, "mpc2000xl-all-file", layerIndex)
{
}

void Mpc2000XlAllFileScreen::function(int i)
{
	init();

	auto loadScreen = dynamic_pointer_cast<LoadScreen>(mpc.screens->getScreenComponent("load"));

	switch (i)
	
	{
	case 2:
	{
		auto sequencesOnly = true;
		mpc::disk::AllLoader allLoader(mpc, loadScreen->getSelectedFile(), sequencesOnly);
		
		auto loadASequenceFromAllScreen = dynamic_pointer_cast<LoadASequenceFromAllScreen>(mpc.screens->getScreenComponent("load-a-sequence-from-all"));
		loadASequenceFromAllScreen->sequencesFromAllFile = allLoader.getSequences();

		loadScreen->fileLoad = 0;
		
		openScreen("load-a-sequence-from-all");
		break;
	}
	case 3:
		openScreen("load");
		break;
	case 4:
	{
		auto sequencesOnly = false;
		mpc::disk::AllLoader allLoader(mpc, loadScreen->getSelectedFile(), sequencesOnly);
		openScreen("sequencer");
		break;
	}
	}
}
