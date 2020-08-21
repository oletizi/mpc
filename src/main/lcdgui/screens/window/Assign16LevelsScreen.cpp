#include "Assign16LevelsScreen.hpp"

#include <Mpc.hpp>

#include <sequencer/Track.hpp>

#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Label.hpp>

#include <hardware/Hardware.hpp>
#include <hardware/TopPanel.hpp>
#include <hardware/Led.hpp>

using namespace mpc::lcdgui::screens::window;
using namespace std;

Assign16LevelsScreen::Assign16LevelsScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "assign-16-levels", layerIndex)
{
}

void Assign16LevelsScreen::open()
{
    displayNote();
    displayParameter();
    displayType();
    displayOriginalKeyPad();
}

void Assign16LevelsScreen::function(int i)
{
    ScreenComponent::function(i);
	switch (i)
	{
    case 4:
	{
		mpc.getHardware().lock()->getTopPanel().lock()->setSixteenLevelsEnabled(true);
		mpc.getHardware().lock()->getLed("sixteen-levels").lock()->light(true);
		openScreen(ls.lock()->getPreviousScreenName());
		break;
	}
    }
}

void Assign16LevelsScreen::turnWheel(int i)
{
    init();

    if (param.compare("note") == 0)
    {
        setNote(note + i);
    }
    else if (param.compare("param") == 0)
    {
        setParam(parameter + i);
    }
    else if (param.compare("type") == 0)
    {
        setType(type + i);
    }
    else if (param.compare("originalkeypad") == 0)
    {
        setOriginalKeyPad(originalKeyPad + i);
    }
}

void Assign16LevelsScreen::setNote(int newNote)
{
    if (newNote < 35 || newNote > 98)
    {
        return;
    }

    note = newNote;
    displayNote();
}

void Assign16LevelsScreen::setParam(int i)
{
    if (i < 0 || i > 1)
    {
        return;
    }

    parameter = i;
    
    displayParameter();
    displayType();
}

void Assign16LevelsScreen::setType(int i)
{
    if (i < 0 || i > 3)
    {
        return;
    }

    type = i;

    displayType();
    displayOriginalKeyPad();
}

void Assign16LevelsScreen::setOriginalKeyPad(int i)
{
    if (i < 3 || i > 12)
    {
        return;
    }

    originalKeyPad = i;
    displayOriginalKeyPad();
}

int Assign16LevelsScreen::getOriginalKeyPad()
{
    return originalKeyPad;
}

int Assign16LevelsScreen::getType()
{
    return type;
}

int Assign16LevelsScreen::getNote()
{
    return note;
}

int Assign16LevelsScreen::getParameter()
{
    return parameter;
}

void Assign16LevelsScreen::displayNote()
{
    init();

    auto pgmNumber = sampler.lock()->getDrumBusProgramNumber(track.lock()->getBus());
    auto program = sampler.lock()->getProgram(pgmNumber).lock();
    auto pn = program->getPadIndexFromNote(note);
    auto sn = program->getNoteParameters(note)->getSndNumber();
    auto soundName = sn == -1 ? "(No sound)" : sampler.lock()->getSoundName(sn);
 
    findField("note").lock()->setText(to_string(note) + "/" + sampler.lock()->getPadName(pn) + "-" + soundName);
}

void Assign16LevelsScreen::displayParameter()
{
    findField("param").lock()->setText(paramNames[parameter]);
}

void Assign16LevelsScreen::displayType()
{
    displayOriginalKeyPad();

    findField("type").lock()->Hide(parameter != 1);
    findLabel("type").lock()->Hide(parameter != 1);

    if (parameter != 1)
    {
        return;
    }

    findField("type").lock()->setText(typeNames[type]);
}

void Assign16LevelsScreen::displayOriginalKeyPad()
{
    findField("originalkeypad").lock()->Hide(!(parameter == 1 && type == 0));
    findLabel("originalkeypad").lock()->Hide(!(parameter == 1 && type == 0));

    if (type != 0)
    {
        return;
    }

    findField("originalkeypad").lock()->setTextPadded(originalKeyPad + 1, " ");
}

void Assign16LevelsScreen::openWindow()
{
    mainScreen();
}
