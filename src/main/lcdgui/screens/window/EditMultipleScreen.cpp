#include "EditMultipleScreen.hpp"

#include <sequencer/ChannelPressureEvent.hpp>
#include <sequencer/ControlChangeEvent.hpp>
#include <sequencer/Event.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/NoteEvent.hpp>
#include <sequencer/PolyPressureEvent.hpp>
#include <sequencer/ProgramChangeEvent.hpp>

#include <lcdgui/screens/StepEditorScreen.hpp>

#include <Util.hpp>

using namespace mpc::lcdgui::screens::window;
using namespace mpc::lcdgui::screens;
using namespace mpc::lcdgui;
using namespace mpc::sequencer;
using namespace moduru::lang;
using namespace std;

EditMultipleScreen::EditMultipleScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "edit-multiple", layerIndex)
{
}

void EditMultipleScreen::open()
{
	updateEditMultiple();
	mpc.addObserver(this); // Subscribe to "padandnote" messages
}

void EditMultipleScreen::close()
{
	mpc.deleteObserver(this);
}

void EditMultipleScreen::function(int i)
{
	ScreenComponent::function(i);
	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));

	auto selectedEvent = stepEditorScreen->getSelectedEvent();
	string paramLetter = stepEditorScreen->getSelectedParameterLetter();
	
	switch (i)
	{
	case 4:
		if (dynamic_pointer_cast<NoteEvent>(selectedEvent.lock()) && track.lock()->getBus() != 0)
		{
			if (paramLetter.compare("a") == 0)
			{
				checkNotes();
			}
			else if (paramLetter.compare("b") == 0)
			{
				for (auto& event : stepEditorScreen->getSelectedEvents())
				{
					if (dynamic_pointer_cast<NoteEvent>(event.lock()))
						dynamic_pointer_cast<NoteEvent>(event.lock())->setVariationTypeNumber(changeVariationTypeNumber);
				}
			}
			else if (paramLetter.compare("c") == 0)
			{
				for (auto& event : stepEditorScreen->getSelectedEvents())
				{
					if (dynamic_pointer_cast<NoteEvent>(event.lock()))
						dynamic_pointer_cast<NoteEvent>(event.lock())->setVariationValue(changeVariationValue);
				}
			}
			else if (paramLetter.compare("d") == 0)
			{
				checkFiveParameters();
			}
			else if (paramLetter.compare("e") == 0)
			{
				checkThreeParameters();
			}
		}

		if (dynamic_pointer_cast<NoteEvent>(selectedEvent.lock()) && track.lock()->getBus() == 0)
		{
			if (paramLetter.compare("a") == 0)
				checkNotes();
			else if (paramLetter.compare("b") == 0)
				checkFiveParameters();
			else if (paramLetter.compare("c") == 0)
				checkThreeParameters();
		}

		if (dynamic_pointer_cast<ControlChangeEvent>(selectedEvent.lock()))
		{
			if (paramLetter.compare("a") == 0)
				checkFiveParameters();
			else if (paramLetter.compare("b") == 0)
				checkThreeParameters();
		}
	
		if (dynamic_pointer_cast<ProgramChangeEvent>(selectedEvent.lock()) || dynamic_pointer_cast<ChannelPressureEvent>(selectedEvent.lock()))
			checkFiveParameters();
	
		if (dynamic_pointer_cast<PolyPressureEvent>(selectedEvent.lock()))
		{
			if (paramLetter.compare("a") == 0)
				checkFiveParameters();
			else if (paramLetter.compare("b") == 0)
				checkThreeParameters();
		}

		stepEditorScreen->clearSelection();
		ls.lock()->openScreen("step-editor");
	}
}

void EditMultipleScreen::turnWheel(int i)
{
	init();

	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));
	auto event = stepEditorScreen->getSelectedEvent();

	string paramLetter = stepEditorScreen->getSelectedParameterLetter();

	if (param.compare("value0") == 0)
	{
		if (dynamic_pointer_cast<NoteEvent>(event.lock()) && track.lock()->getBus() != 0)
		{
			if (paramLetter.compare("a") == 0)
			{
				if (changeNoteToNumber == 98)
					return;

				setChangeNoteToIndex(changeNoteToNumber + i);
			}
			else if (paramLetter.compare("b") == 0)
			{
				setChangeVariationTypeNumber(changeVariationTypeNumber + i);
			}
			else if (paramLetter.compare("c") == 0)
			{
				setChangeVariationValue(changeVariationValue + i);
			}
			else if (paramLetter.compare("d") == 0 || paramLetter.compare("e") == 0)
			{
				setEditTypeNumber(editTypeNumber + i);
			}
		}
		else if (dynamic_pointer_cast<NoteEvent>(event.lock()) && track.lock()->getBus() == 0)
		{
			if (paramLetter.compare("a") == 0)
				setChangeNoteToIndex(changeNoteToNumber + i);
			else if (paramLetter.compare("b") == 0 || paramLetter.compare("c") == 0)
				setEditTypeNumber(editTypeNumber + i);
		}
		else if (dynamic_pointer_cast<ProgramChangeEvent>(event.lock())
			|| dynamic_pointer_cast<PolyPressureEvent>(event.lock())
			|| dynamic_pointer_cast<ChannelPressureEvent>(event.lock())
			|| dynamic_pointer_cast<ControlChangeEvent>(event.lock()))
		{
			setEditTypeNumber(editTypeNumber + i);
		}
	}
	else if (param.compare("value1") == 0)
	{
		setEditValue(editValue + 1);
	}

	updateEditMultiple();
}

void EditMultipleScreen::checkThreeParameters()
{
	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));

	for (auto& event : stepEditorScreen->getSelectedEvents())
	{

		auto note = dynamic_pointer_cast<NoteEvent>(event.lock());
		auto controlChange = dynamic_pointer_cast<ControlChangeEvent>(event.lock());
		auto polyPressure = dynamic_pointer_cast<PolyPressureEvent>(event.lock());

		if (note)
			note->setVelocity(editValue);
		else if (controlChange)
			controlChange->setAmount(editValue);
		else if (polyPressure)
			polyPressure->setAmount(editValue);
	}
}

void EditMultipleScreen::checkFiveParameters()
{
	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));

	for (auto& event : stepEditorScreen->getSelectedEvents())
	{
		auto note = dynamic_pointer_cast<NoteEvent>(event.lock());
		auto programChange = dynamic_pointer_cast<ProgramChangeEvent>(event.lock());
		auto controlChange = dynamic_pointer_cast<ControlChangeEvent>(event.lock());
		auto channelPressure = dynamic_pointer_cast<ChannelPressureEvent>(event.lock());
		auto polyPressure = dynamic_pointer_cast<PolyPressureEvent>(event.lock());

		if (note)
			note->setDuration(editValue);
		else if (programChange)
			programChange->setProgram(editValue);
		else if (controlChange)
			controlChange->setController(editValue);
		else if (channelPressure)
			channelPressure->setAmount(editValue);
		else if (polyPressure)
			polyPressure->setNote(editValue);
	}
}

void EditMultipleScreen::checkNotes()
{
	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));
	for (auto& event : stepEditorScreen->getSelectedEvents())
	{
		auto note = dynamic_pointer_cast<NoteEvent>(event.lock());

		if (note)
			note->setNote(changeNoteToNumber);
	}
}

void EditMultipleScreen::setEditTypeNumber(int i)
{
	if (i < 0 || i > 3)
		return;

	editTypeNumber = i;
	updateEditMultiple();
}

void EditMultipleScreen::updateEditMultiple()
{
	init();

	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));
	auto event = stepEditorScreen->getSelectedEvent().lock();
	auto letter = stepEditorScreen->getSelectedParameterLetter();

	if (dynamic_pointer_cast<NoteEvent>(event) && track.lock()->getBus() != 0)
	{
		if (letter.compare("a") == 0 || letter.compare("b") == 0 || letter.compare("c") == 0)
		{
			findLabel("value1").lock()->Hide(true);
			findField("value1").lock()->Hide(true);
			findLabel("value0").lock()->Hide(false);
			findLabel("value0").lock()->setLocation(xPosSingle, yPosSingle);
		
			if (letter.compare("a") == 0)
			{
				findLabel("value0").lock()->setText(singleLabels[0]);
				findField("value0").lock()->setSize(6 * 6 + 1, 9);
				findField("value0").lock()->setText(to_string(changeNoteToNumber) + "/" + sampler.lock()->getPadName(program.lock()->getPadIndexFromNote(changeNoteToNumber)));
			}
			else if (letter.compare("b") == 0)
			{
				findLabel("value0").lock()->setText(singleLabels[1]);
				findField("value0").lock()->setSize(3 * 6 + 1, 9);
				findField("value0").lock()->setText(noteVariationParameterNames[changeVariationTypeNumber]);
			}
			else if (letter.compare("c") == 0)
			{
				findLabel("value0").lock()->setText(singleLabels[2]);
			
				if (changeVariationTypeNumber == 0)
				{
					findField("value0").lock()->setSize(4 * 6 + 1, 9);
					findField("value0").lock()->setLocation(45, findField("value0").lock()->getY());
					auto noteVarValue = (changeVariationValue * 2) - 128;

					if (noteVarValue < -120)
					{
						noteVarValue = -120;
					}
					else if (noteVarValue > 120)
					{
						noteVarValue = 120;
					}

					if (noteVarValue == 0)
					{
						findField("value0").lock()->setTextPadded(0, " ");
					}
					else if (noteVarValue < 0)
					{
						findField("value0").lock()->setText("-" + StrUtil::padLeft(to_string(abs(noteVarValue)), " ", 3));
					}
					else if (noteVarValue > 0)
					{
						findField("value0").lock()->setText("+" + StrUtil::padLeft(to_string(noteVarValue), " ", 3));
					}
				}

				if (changeVariationTypeNumber == 1 || changeVariationTypeNumber == 2)
				{
					auto noteVarValue = changeVariationValue;
				
					if (noteVarValue > 100)
						noteVarValue = 100;

					findField("value0").lock()->setText(StrUtil::padLeft(to_string(noteVarValue), " ", 3));
					findField("value0").lock()->setSize(3 * 6 + 1, 9);
					findField("value0").lock()->setLocation(51, findField("value0").lock()->getY());
				}
				else if (changeVariationTypeNumber == 3)
				{
					findField("value0").lock()->setSize(4 * 6 + 1, 9);
					findField("value0").lock()->setLocation(45, findField("value0").lock()->getY());

					auto noteVarValue = changeVariationValue - 50;
					
					if (noteVarValue > 50)
						noteVarValue = 50;
					
					if (noteVarValue < 0)
						findField("value0").lock()->setText("-" + StrUtil::padLeft(to_string(abs(noteVarValue)), " ", 2));
					else if (noteVarValue > 0)
						findField("value0").lock()->setText("+" + StrUtil::padLeft(to_string(noteVarValue), " ", 2));
					else
						findField("value0").lock()->setTextPadded("0", " ");
				}
			}

			findLabel("value0").lock()->setSize(findLabel("value0").lock()->getText().length() * 6 + 1, 9);
			findField("value0").lock()->Hide(false);
			findField("value0").lock()->setLocation(xPosSingle + findLabel("value0").lock()->getW(), yPosSingle);
		}
		else if (letter.compare("d") == 0 || letter.compare("e") == 0)
		{
			updateDouble();
		}
	}

	if (dynamic_pointer_cast<NoteEvent>(event) && track.lock()->getBus() == 0)
	{
		if (letter.compare("a") == 0)
		{
			findLabel("value1").lock()->Hide(true);
			findField("value1").lock()->Hide(true);
			findLabel("value0").lock()->Hide(false);
			findLabel("value0").lock()->setLocation(xPosSingle, yPosSingle);
			findLabel("value0").lock()->setText(singleLabels[0]);
			findField("value0").lock()->setSize(8 * 6 + 1, 9);
			findField("value0").lock()->setText((StrUtil::padLeft(to_string(changeNoteToNumber), " ", 3) + "(" + mpc::Util::noteNames()[changeNoteToNumber]) + ")");
			findLabel("value0").lock()->setSize(findLabel("value0").lock()->GetTextEntryLength() * 6 + 1, 9);
			findField("value0").lock()->Hide(false);
			findField("value0").lock()->setLocation(xPosSingle + findLabel("value0").lock()->getW(), yPosSingle);
		}
		else if (letter.compare("b") == 0 || letter.compare("c") == 0)
		{
			updateDouble();
		}
	}
	if (dynamic_pointer_cast<ProgramChangeEvent>(event)
		|| dynamic_pointer_cast<PolyPressureEvent>(event)
		|| dynamic_pointer_cast<ChannelPressureEvent>(event)
		|| dynamic_pointer_cast<ControlChangeEvent>(event))
	{
		updateDouble();
	}
}

void EditMultipleScreen::updateDouble()
{
	auto stepEditorScreen = dynamic_pointer_cast<StepEditorScreen>(mpc.screens->getScreenComponent("step-editor"));

	findLabel("value0").lock()->Hide(false);
	findLabel("value1").lock()->Hide(false);
	findField("value0").lock()->Hide(false);
	findField("value1").lock()->Hide(false);
	findLabel("value0").lock()->setText(doubleLabels[0]);
	findLabel("value1").lock()->setText(doubleLabels[1]);
	findLabel("value0").lock()->setSize(findLabel("value0").lock()->GetTextEntryLength() * 6 + 1, 9);
	findLabel("value0").lock()->setLocation(xPosDouble[0], yPosDouble[0]);
	findLabel("value1").lock()->setSize(findLabel("value1").lock()->GetTextEntryLength() * 6 + 1, 9);
	findLabel("value1").lock()->setLocation(xPosDouble[1], yPosDouble[1]);
	findField("value0").lock()->setLocation((xPosDouble[0] + findLabel("value0").lock()->getW()), yPosDouble[0]);
	findField("value1").lock()->setLocation((xPosDouble[1] + findLabel("value1").lock()->getW()), yPosDouble[1]);
	findField("value0").lock()->setText(editTypeNames[editTypeNumber]);
	findField("value1").lock()->setText(to_string(editValue));
	findField("value0").lock()->setSize(findField("value0").lock()->GetTextEntryLength() * 6 + 1, 9);
	findField("value1").lock()->setSize(findField("value1").lock()->GetTextEntryLength() * 6 + 1, 9);
}

void EditMultipleScreen::update(moduru::observer::Observable* o, nonstd::any arg)
{
	string s = nonstd::any_cast<string>(arg);

	if (s.compare("padandnote") == 0)
	{
		if (mpc.getNote() != 34)
		{
			changeNoteToNumber = mpc.getNote();
			updateEditMultiple();
		}
	}
}

void EditMultipleScreen::setChangeNoteToIndex(int i)
{
	if (i < 0 || i > 127)
		return;

	changeNoteToNumber = i;
	updateEditMultiple();
}

void EditMultipleScreen::setChangeVariationTypeNumber(int i)
{
	if (i < 0 || i > 3)
		return;

	changeVariationTypeNumber = i;
	updateEditMultiple();
}

void EditMultipleScreen::setChangeVariationValue(int i)
{
	if (i < 0 || i > 128)
		return;

	if (changeVariationTypeNumber != 0 && i > 100)
		i = 100;

	changeVariationValue = i;
	updateEditMultiple();
}

void EditMultipleScreen::setEditValue(int i)
{
	if (i < 0 || i > 127)
		return;

	editValue = i;
	updateEditMultiple();
}
