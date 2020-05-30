#include "TrMoveScreen.hpp"

#include "BarCopyScreen.hpp"
#include "EditSequenceScreen.hpp"

#include <sequencer/Track.hpp>

using namespace mpc::lcdgui;
using namespace mpc::lcdgui::screens;
using namespace moduru::lang;
using namespace std;

TrMoveScreen::TrMoveScreen(const int& layer)
	: ScreenComponent("trmove", layer)
{
}

void TrMoveScreen::open()
{
	//May need to happen more often
	ls.lock()->setCurrentBackground("trmove");
	findLabel("selecttrack").lock()->setText("Select track");
	findLabel("tomove").lock()->setText("to move.");

	displaySq();
	displayTrFields();
	displayTrLabels();
	auto barCopyScreen = dynamic_pointer_cast<BarCopyScreen>(Screens::getScreenComponent("barcopy"));
	setSq(barCopyScreen->getFromSq());
}

void TrMoveScreen::turnWheel(int i)
{
	init();
	if (param.find("tr") != string::npos && i > 0)
	{
		goUp();
	}
	else if (param.find("tr") != string::npos && i < 0)
	{
		goDown();
	}
	else if (param.compare("sq") == 0)
	{
		setSq(sq + i);
	}
}

void TrMoveScreen::up()
{
	init();
	if (param.find("tr") != string::npos)
	{
		goUp();
	}
}

void TrMoveScreen::down()
{
	init();
	if (param.find("tr") != string::npos)
	{
		goDown();
	}
}

void TrMoveScreen::left()
{
	init();
	if (isSelected() && param.compare("tr0") == 0)
	{
		return;
	}
	else if (!isSelected() && param.compare("sq") == 0)
	{
		return;
	}
	BaseControls::left();
	init();

	if (param.compare("sq") == 0)
	{
		ls.lock()->setFunctionKeysArrangement(0);
	}
}

void TrMoveScreen::right()
{
	init();
	if (isSelected() && param.compare("tr0") == 0)
	{
		return;
	}
	
	if (!isSelected() && param.compare("tr1") == 0)
	{
		return;
	}
	
	BaseControls::right();
	init();
	
	if (param.compare("sq") != 0 && !isSelected())
	{
		ls.lock()->setFunctionKeysArrangement(1);
	}
}

void TrMoveScreen::function(int i)
{
	init();
	switch (i)
	{
	case 0:
	{
		auto editSequenceScreen = dynamic_pointer_cast<EditSequenceScreen>(Screens::getScreenComponent("editsequence"));
		editSequenceScreen->setFromSq(sq);
		ls.lock()->openScreen("edit");
		break;
	}
	case 1:
	{		auto barCopyScreen = dynamic_pointer_cast<BarCopyScreen>(Screens::getScreenComponent("barcopy"));
	barCopyScreen->setFromSq(sq);
	ls.lock()->openScreen("barcopy");
	break;
	}
	case 3:
		ls.lock()->openScreen("user");
		break;
	case 4:
		if (isSelected()) {
			cancel();
		}
		ls.lock()->setFocus("tr1");
		break;
	case 5:
		if (param.compare("sq") == 0)
		{
			break;
		}

		if (isSelected())
		{
			auto sequence = sequencer.lock()->getSequence(sq).lock();
			insert(sequence.get());
			ls.lock()->setFocus("tr1");
			ls.lock()->setFunctionKeysArrangement(1);
		}
		else
		{
			select();
			ls.lock()->setFocus("tr0");
		}
		break;
	}
}

void TrMoveScreen::displayTrLabels()
{
	string tr0 = "";
	string tr1 = "";
	string tr0Name = "";
	string tr1Name = "";

	auto tr0Index = currentTrackIndex - 1;
	auto tr1Index = 0;
	
	if (isSelected() && tr0Index >= selectedTrackIndex)
	{
		tr0Index++;
	}

	tr1Index = tr0Index + 2;

	if (isSelected())
	{
		tr1Index--;

		if (tr0Index == selectedTrackIndex)
		{
			tr0Index--;
		}
		
		if (tr1Index == selectedTrackIndex)
		{
			tr1Index++;
		}
	}

	auto sequence = sequencer.lock()->getSequence(sq).lock();

	if (tr0Index >= 0)
	{
		tr0Name = sequence->getTrack(tr0Index).lock()->getName();
		tr0 += "Tr:" + StrUtil::padLeft(to_string(tr0Index + 1), "0", 2) + "-" + tr0Name;
	}
	else
	{
		tr0 = "";
	}
	if (tr1Index < 64) {
		tr1Name = sequence->getTrack(tr1Index).lock()->getName();
		tr1 += "Tr:" + StrUtil::padLeft(to_string(tr1Index + 1), "0", 2) + "-" + tr1Name;
	}
	else {
		tr1 = "";
	}
	if (tr0.compare("") == 0) {
		findLabel("tr0").lock()->Hide(true);
	}
	else {
		findLabel("tr0").lock()->Hide(false);
		findLabel("tr0").lock()->setText(tr0);
	}
	if (tr1.compare("") == 0) {
		findLabel("tr1").lock()->Hide(true);
	}
	else {
		findLabel("tr1").lock()->Hide(false);
		findLabel("tr1").lock()->setText(tr1);
	}
}

void TrMoveScreen::displayTrFields()
{
	auto sequence = sequencer.lock()->getSequence(sq).lock();
	
	if (isSelected())
	{
		findLabel("selecttrack").lock()->Hide(true);
		findLabel("tomove").lock()->Hide(true);
		findField("tr1").lock()->Hide(true);
		findField("tr0").lock()->Hide(false);

		auto tr0Name = sequence->getTrack(selectedTrackIndex).lock()->getName();
		
		if (tr0Name.length() < 10)
		{
			tr0Name = StrUtil::padRight(tr0Name, " ", 9) + u8"\u00CD";
		}
		
		findField("tr0").lock()->setText("Tr:" + StrUtil::padLeft(to_string(selectedTrackIndex + 1), "0", 2) + "-" + tr0Name);
		ls.lock()->setFunctionKeysArrangement(2);
	}
	else
	{
		findLabel("selecttrack").lock()->Hide(false);
		findLabel("tomove").lock()->Hide(false);
		findField("tr0").lock()->Hide(true);
		findField("tr1").lock()->Hide(false);

		findField("tr1").lock()->setText("Tr:" + StrUtil::padLeft(to_string(currentTrackIndex + 1), "0", 2) + "-" + sequence->getTrack(currentTrackIndex).lock()->getName());
		
		if (ls.lock()->getFocus().compare(findField("tr1").lock()->getName()) == 0)
		{
			ls.lock()->setFunctionKeysArrangement(1);
		}
		else
		{
			ls.lock()->setFunctionKeysArrangement(0);
		}
	}
}

void TrMoveScreen::displaySq()
{
	findLabel("sq").lock()->SetDirty();
	findField("sq").lock()->setText(StrUtil::padLeft(to_string(sq + 1), "0", 2) + "-" + sequencer.lock()->getSequence(sq).lock()->getName());
}

bool TrMoveScreen::isSelected()
{
	if (selectedTrackIndex != -1)
	{
		return true;
	}
	return false;
}

void TrMoveScreen::goUp()
{
	if (currentTrackIndex == 0)
	{
		return;
	}
	
	currentTrackIndex--;
	
	displayTrLabels();
	displayTrFields();
}

void TrMoveScreen::goDown()
{
	if (currentTrackIndex == 63)
	{
		return;
	}
	currentTrackIndex++;
	displayTrLabels();
	displayTrFields();
}

void TrMoveScreen::setSq(int i)
{
	if (i < 0 || i > 98)
	{
		return;
	}
	sq = i;
	displaySq();
}

void TrMoveScreen::select()
{
	selectedTrackIndex = currentTrackIndex;
	displayTrLabels();
	displayTrFields();
}

void TrMoveScreen::cancel()
{
	selectedTrackIndex = -1;
	displayTrLabels();
	displayTrFields();
}

void TrMoveScreen::insert(mpc::sequencer::Sequence* s)
{
	s->moveTrack(selectedTrackIndex, currentTrackIndex);
	selectedTrackIndex = -1;
	displayTrLabels();
	displayTrFields();
}
