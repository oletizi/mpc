#include "Field.hpp"

#include "Label.hpp"

#include <Mpc.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <ui/NameGui.hpp>
#include <lcdgui/TwoDots.hpp>

#include <lang/StrUtil.hpp>

#include <file/File.hpp>

#ifdef __linux__
#include <climits>
#endif // __linux__

#include <stdexcept>

using namespace mpc::lcdgui;
using namespace std;

Field::Field(const string& name, int x, int y, int width)
	: TextComp(name)
{

	split = false;
	focusable = true;
	inverted = false;
	focus = false;
	Hide(false);
	this->name = name;
	this->width = width - 1;
	setLocation(x, y);
	setSize(width, FONT_HEIGHT);
}

const int Field::BLINKING_RATE;

void Field::takeFocus(string prev)
{
	auto layeredScreen = mpc::Mpc::instance().getLayeredScreen().lock();
	csn = layeredScreen->getCurrentScreenName();
	focus = true;
	inverted = true;
	
	auto focusEvent = layeredScreen->getFocus();
	auto focusField = layeredScreen->lookupField(focusEvent);
	if (csn.compare("trim") == 0 || csn.compare("loop") == 0) {
		if (focusEvent.compare("st") == 0 || focusEvent.compare("to") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(0, true);
		}
		else if (focusEvent.compare("end") == 0 || focusEvent.compare("endlengthvalue") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(1, true);
		}
	}
	if (csn.compare("startfine") == 0 || csn.compare("endfine") == 0 || csn.compare("looptofine") == 0 || csn.compare("loopendfine") == 0) {
		if (focusEvent.compare("start") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(2, true);
		}
		else if (focusEvent.compare("end") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(2, true);
		}
		else if (focusEvent.compare("to") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(2, true);
		}
		else if (focusEvent.compare("lngth") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(3, true);
		}
	}
	SetDirty();
}

void Field::loseFocus(string next)
{
	focus = false;
	inverted = false;
	auto focusEvent = getName();

	auto layeredScreen = mpc::Mpc::instance().getLayeredScreen().lock();

	csn = layeredScreen->getCurrentScreenName();
	if (csn.compare("trim") == 0 || csn.compare("loop") == 0) {
		if (focusEvent.compare("st") == 0 || focusEvent.compare("to") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(0, false);
		}
		else if (focusEvent.compare("end") == 0 || focusEvent.compare("endlengthvalue") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(1, false);
		}
	}
	else if (csn.compare("startfine") == 0 || csn.compare("endfine") == 0 || csn.compare("looptofine") == 0 || csn.compare("loopendfine") == 0) {
		if (focusEvent.compare("start") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(2, false);
		}
		else if (focusEvent.compare("end") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(2, false);
		}
		else if (focusEvent.compare("to") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(2, false);
		}
		else if (focusEvent.compare("lngth") == 0) {
			layeredScreen->getTwoDots().lock()->setSelected(3, false);
		}
	}

	SetDirty();
}


void Field::setSplit(bool b)
{
	if (split == b) return;
	split = b;
	if (split) {
		this->setOpaque(false);
		if (letters.size() != 0) {
			for (auto& l : letters)
				delete l;
		}
		letters.clear();
		letters = vector<Label*>(getText().length());
		activeSplit = letters.size() - 1;
		//auto x = getLocation())->x;
		//auto y = npc(this->getLocation())->y;

		for (int i = 0; i < letters.size(); i++) {
			//letters[i] = new Label(GetGUI()->GetPlug());
			//letters[i]->setFocusable(false);
			//GetGUI()->AttachControl(letters[i]);
		}
		setText(getText());
		redrawSplit();
		//SetDirty(true);
	}
	else {
		if (letters.size() == 0)
			return;

		setOpaque(true);
		//for (int i = 0; i < letters.size(); i++)
			//GetGUI()->DetachControl(letters[i]);
		activeSplit = 0;
		letters.clear();
		//SetDirty(true);
	}
}

void Field::redrawSplit()
{
	for (int i = 0; i < letters.size(); i++) {
		//letters[i]->setForeground(i < activeSplit);
		//letters[i]->setBackground(i < activeSplit ? mpc::maingui::Constants::LCD_ON() : mpc::maingui::Constants::LCD_OFF());
		//letters[i]->setOpaque(i < activeSplit);
	}
	//SetDirty(true);
}

bool Field::isSplit()
{
    return split;
}

int Field::getActiveSplit()
{
    return activeSplit;
}

bool Field::setActiveSplit(int i)
{
	if (i < 1 || i + 1 > letters.size()) return false;
	activeSplit = i;
	redrawSplit();
	return true;
}

bool Field::enableTypeMode()
{
    if(typeModeEnabled)
        return false;

    typeModeEnabled = true;
	//oldText = this->getText();
    //setFont(mpc2000fontunderline);
    //setFontColor(gui::Constants::LCD_ON);
	//setTransparency(true);
    setText("");
    return true;
}

int Field::enter()
{
	auto value = INT_MAX;
    if(!typeModeEnabled)
        return value;

    //setFont(mpc2000font);
    //setFontColor(Constants::LCD_OFF);
	//setTransparency(false);
    typeModeEnabled = false;
    try {
	//	string valueString = getText();
	//	value = stoi(valueString);
	}
	catch (std::invalid_argument& e) {
		printf("Field.enter ERROR: %s", e.what());
        return value;
    }
    //setText(oldText.c_str());
    return value;
}

void Field::type(int i)
{
}

bool Field::isTypeModeEnabled()
{
    return typeModeEnabled;
}

void Field::disableTypeMode()
{
    if(!typeModeEnabled)
        return;

    typeModeEnabled = false;
    //setFontColor(Constants::LCD_OFF);
	//setTransparency(false);
    //setText(oldText.c_str());
}

void Field::startBlinking()
{
    blinking = true;
}

void Field::stopBlinking()
{
    blinking = false;
}

bool Field::getBlinking()
{
    return this->blinking;
}

void Field::setFocusable(bool b) {
	focusable = b;
}

bool Field::isFocusable() {
	return focusable;
}

bool Field::hasFocus() {
	return focus;
}

Field::~Field()
{
	for (auto& l : letters) {
		delete l;
	}
}
