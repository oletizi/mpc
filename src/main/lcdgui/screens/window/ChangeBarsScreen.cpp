#include "ChangeBarsScreen.hpp"

#include <lcdgui/LayeredScreen.hpp>
#include <sequencer/Sequence.hpp>

using namespace mpc::lcdgui::screens::window;
using namespace std;

ChangeBarsScreen::ChangeBarsScreen(const int layerIndex)
	: ScreenComponent("change-bars", layerIndex)
{
}

void ChangeBarsScreen::open()
{
	setAfterBar(0);
	setNumberOfBars(0);
	setFirstBar(0);
	setLastBar(0);
}

void ChangeBarsScreen::function(int i)
{
    BaseControls::function(i);
	auto seq = sequencer.lock()->getActiveSequence().lock();

	switch (i)
	{
	case 1:
		seq->insertBars(numberOfBars, afterBar);
		ls.lock()->openScreen("sequencer");
		break;
	case 4:
		seq->deleteBars(firstBar, lastBar);
		ls.lock()->openScreen("sequencer");
		break;
	}
}

void ChangeBarsScreen::turnWheel(int i)
{
	init();

	if (param.compare("afterbar") == 0)
	{
		setAfterBar(afterBar + i);
	}
	else if (param.compare("numberofbars") == 0)
	{
		setNumberOfBars(numberOfBars + i);
	}
	else if (param.compare("firstbar") == 0)
	{
		setFirstBar(firstBar + i);
	}
	else if (param.compare("lastbar") == 0)
	{
		setLastBar(lastBar + i);
	}
}

void ChangeBarsScreen::displayFirstBar()
{
	findField("firstbar").lock()->setText(to_string(firstBar + 1));
}

void ChangeBarsScreen::displayLastBar()
{
	findField("lastbar").lock()->setText(to_string(lastBar + 1));
}

void ChangeBarsScreen::displayNumberOfBars()
{
	findField("numberofbars").lock()->setText(to_string(numberOfBars));
}

void ChangeBarsScreen::displayAfterBar()
{
	findField("afterbar").lock()->setText(to_string(afterBar));
}

void ChangeBarsScreen::setLastBar(int i)
{
	auto seq = sequencer.lock()->getActiveSequence().lock();

	if (i < 0 || i > seq->getLastBar())
	{
		return;
	}

	lastBar = i;

	if (lastBar < firstBar)
	{
		setFirstBar(lastBar);
	}

	displayLastBar();
}

void ChangeBarsScreen::setFirstBar(int i)
{
	auto seq = sequencer.lock()->getActiveSequence().lock();

	if (i < 0 || i > seq->getLastBar())
	{
		return;
	}

	firstBar = i;
	
	displayFirstBar();

	if (firstBar > lastBar)
	{
		setLastBar(firstBar);
	}
}

void ChangeBarsScreen::setNumberOfBars(int i)
{
	auto seq = sequencer.lock()->getActiveSequence().lock();

	if (i < 0 || i > (998 - seq->getLastBar()))
	{
		return;
	}

	numberOfBars = i;
	displayNumberOfBars();
}

void ChangeBarsScreen::setAfterBar(int i)
{
	auto seq = sequencer.lock()->getActiveSequence().lock();

	if (i < 0 || i >= seq->getLastBar())
	{
		return;
	}
	
	afterBar = i;
	displayAfterBar();
}
