#include "CopySequenceScreen.hpp"

#include <lang/StrUtil.hpp>

using namespace mpc::lcdgui::screens::dialog;
using namespace std;

CopySequenceScreen::CopySequenceScreen(const int layerIndex)
	: ScreenComponent("copy-sequence", layerIndex)
{
}

void CopySequenceScreen::open()
{
	displaySq0();
	displaySq1();
}

void CopySequenceScreen::function(int i)
{
	init();
	
	switch (i) {
	case 2:
		sequencer.lock()->copySequence(sq0, sq1);
		ls.lock()->openScreen("sequencer");
		break;
	case 4:
		sequencer.lock()->copySequence(sq0, sq1);
		ls.lock()->openScreen("sequencer");
		break;
	}
}

void CopySequenceScreen::turnWheel(int i)
{
    init();
	if (param.find("0") != string::npos)
	{
		setSq0(sq0 + i);
	}
	else if (param.find("1") != string::npos)
	{
		setSq1(sq1 + i);
	}
}

void CopySequenceScreen::setSq0(int i)
{
	if (i < 0 || i > 98)
	{
		return;
	}
	sq0 = i;
	displaySq0();
}

void CopySequenceScreen::setSq1(int i)
{
	if (i < 0 || i > 98)
	{
		return;
	}
	sq1 = i;
	displaySq1();
}

void CopySequenceScreen::displaySq0()
{
	auto sq0Name = sequencer.lock()->getSequence(sq0).lock()->getName();
	findField("sq0").lock()->setText(moduru::lang::StrUtil::padLeft(to_string(sq0 + 1), " ", 2) + "-" + sq0Name);
}

void CopySequenceScreen::displaySq1()
{
	auto sq1Name = sequencer.lock()->getSequence(sq1).lock()->getName();
	findField("sq1").lock()->setText(moduru::lang::StrUtil::padLeft(to_string(sq1 + 1), " ", 2) + "-" + sq1Name);
}
