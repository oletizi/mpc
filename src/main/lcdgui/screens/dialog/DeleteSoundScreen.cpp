#include "DeleteSoundScreen.hpp"

using namespace mpc::lcdgui::screens::dialog;
using namespace std;

DeleteSoundScreen::DeleteSoundScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "delete-sound", layerIndex)
{
}

void DeleteSoundScreen::open()
{
	displaySnd();
}

void DeleteSoundScreen::function(int i)
{
    init();
	
	switch (i)
	{
    case 2:
        ls.lock()->openScreen("delete-all-sound");
        break;
    case int (3):
        ls.lock()->openScreen("sound");
        break;
    case 4:
        sampler.lock()->deleteSample(sampler.lock()->getSoundIndex());

		if (sampler.lock()->getSoundIndex() > sampler.lock()->getSoundCount() - 1)
		{
			sampler.lock()->setSoundIndex(sampler.lock()->getSoundCount() - 1);
		}
		
		if (sampler.lock()->getSoundCount() > 0)
		{
			ls.lock()->openScreen("sound");
		}
		else
		{
			ls.lock()->openScreen(sampler.lock()->getPreviousScreenName());
		}
        break;
    }
}

void DeleteSoundScreen::turnWheel(int i)
{
	init();
	if (param.compare("snd") == 0)
	{
		sampler.lock()->setSoundIndex(sampler.lock()->getSoundIndex() + i);
		displaySnd();
	}
}

void DeleteSoundScreen::displaySnd()
{
	if (!sampler.lock()->getSound().lock())
	{
		return;
	}

	findField("snd").lock()->setText(sampler.lock()->getSound().lock()->getName());
}
