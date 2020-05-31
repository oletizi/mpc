#include <lcdgui/MixerStrip.hpp>

#include <lcdgui/Knob.hpp>
#include <lcdgui/Label.hpp>
#include <lcdgui/MixerFaderBackground.hpp>
#include <lcdgui/MixerTopBackground.hpp>
#include <lcdgui/MixerFader.hpp>
#include <lcdgui/screens/MixerScreen.hpp>
#include <lcdgui/Screens.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::lcdgui::screens;
using namespace mpc::lcdgui;
using namespace moduru::lang;

MixerStrip::MixerStrip(int columnIndex, int bank)
	: Component("mixer-strip")
{
	this->columnIndex = columnIndex;

	yPos0indiv = 0;
	yPos1indiv = 4;
	yPos0fx = 2;
	yPos1fx = 2;
	selection = -1;

	auto x1 = 4 + (columnIndex * 15);
	addChild(move(make_shared<MixerTopBackground>(MRECT(x1, 0, x1 + 14, 13))));
	addChild(move(make_shared<MixerFaderBackground>(MRECT(x1, 14, x1 + 14, 50))));

	auto x2 = 5 + (columnIndex * 15);
	addChild(move(make_shared<Knob>(MRECT(x2, 1, x2 + 12, 12))));

	auto x3 = 12 + (columnIndex * 15);
	addChild(move(make_shared<MixerFader>(MRECT(x3, 15, x3 + 4, 49))));


	for (int i = 0; i < 5; i++)
	{
		auto xPos = 5 + (columnIndex * 15);

		if (i == 1)
		{
			xPos += 6;
		}

		auto yPos = 3 + (i * 13);
		if (i >= 1)
		{
			yPos -= 13;
		}

		auto label = dynamic_pointer_cast<Label>(addChild(move(make_shared<Label>(letters[columnIndex] + to_string(i), "", xPos, yPos, 6))).lock());
		labels.push_back(label);
	}

	labels[1].lock()->setOpaque(false);
	labels[2].lock()->setOpaque(true);
	labels[2].lock()->setText(abcd[bank]);

	auto padName = StrUtil::padLeft(to_string(columnIndex + 1), "0", 2);
	labels[3].lock()->setText(padName.substr(0, 1));
	labels[4].lock()->setText(padName.substr(1, 2));

	initLabels();
	setColors();
}

void MixerStrip::setValueA(int i)
{
    findKnob().lock()->setValue(i);
}

void MixerStrip::setValueB(int i)
{
    findMixerFader().lock()->setValue(i);
}

void MixerStrip::initLabels()
{
	auto mixerScreen = dynamic_pointer_cast<MixerScreen>(Screens::getScreenComponent("mixer"));

	if (mixerScreen->getTab() == 0)
	{
		findKnob().lock()->Hide(false);
		labels[0].lock()->Hide(true);
		labels[1].lock()->Hide(true);
		labels[2].lock()->Hide(false);
		labels[3].lock()->Hide(false);
		labels[4].lock()->Hide(false);
	}
	else if (mixerScreen->getTab() == 1)
	{
		findKnob().lock()->Hide(true);
		labels[0].lock()->Hide(false);
		labels[1].lock()->Hide(false);
		labels[0].lock()->setLocation(xPos0indiv[columnIndex] - 1, yPos0indiv);
		labels[1].lock()->setLocation(xPos1indiv[columnIndex] - 1, yPos1indiv);
	}
	else if (mixerScreen->getTab() == 2)
	{
		findKnob().lock()->Hide(true);
		labels[0].lock()->Hide(false);
		labels[1].lock()->Hide(false);
		labels[0].lock()->setLocation(xPos0fx[columnIndex], yPos0fx);
		labels[1].lock()->setLocation(xPos1fx[columnIndex], yPos1fx);
	}
}

void MixerStrip::setColors()
{
	if (selection == -1)
	{
		for (auto& tf : labels)
		{
			tf.lock()->setInverted(false);
		}
		
		findMixerTopBackground().lock()->Hide(true);
		findMixerFaderBackground().lock()->Hide(true);
		findKnob().lock()->setColor(true);
		findMixerFader().lock()->Hide(false);
		findMixerFader().lock()->setColor(true);
	}
	else if (selection == 0)
	{
		labels[0].lock()->setInverted(true);
		labels[1].lock()->setInverted(true);
		labels[2].lock()->setInverted(false);
		labels[3].lock()->setInverted(false);
		labels[4].lock()->setInverted(false);
		findMixerTopBackground().lock()->Hide(false);
		findMixerFaderBackground().lock()->Hide(true);
		findKnob().lock()->setColor(false);
		findMixerFader().lock()->Hide(false);
		findMixerFader().lock()->setColor(true);
	}
    else if (selection == 1)
	{
        labels[0].lock()->setInverted(false);
        labels[1].lock()->setInverted(false);
        labels[2].lock()->setInverted(true);
        labels[3].lock()->setInverted(true);
        labels[4].lock()->setInverted(true);
		findMixerTopBackground().lock()->Hide(true); 
		findMixerFaderBackground().lock()->Hide(false);
        findKnob().lock()->setColor(true);
		findMixerFader().lock()->Hide(false);
		findMixerFader().lock()->setColor(false);
    }
}

void MixerStrip::setSelection(int i)
{
    selection = i;
    setColors();
}

void MixerStrip::setValueAString(string str)
{
	auto mixerScreen = dynamic_pointer_cast<MixerScreen>(Screens::getScreenComponent("mixer"));

	if (mixerScreen->getTab() == 1)
	{
		if (str.length() == 1)
		{
			labels[0].lock()->setText(str);
			labels[0].lock()->setLocation(xPos0indiv[columnIndex] + 2, yPos0indiv + 2);
			labels[1].lock()->setText("");
		}
		else if (str.length() == 2)
		{
			labels[0].lock()->setLocation(xPos0indiv[columnIndex], yPos0indiv);
			labels[0].lock()->setText(str.substr(0, 1));
			labels[1].lock()->setText(str.substr(1, 2));
		}
	}
	else if (mixerScreen->getTab() == 2)
	{
		labels[0].lock()->setText(str.substr(0, 1));
		labels[1].lock()->setText(str.substr(1, 2));
	}
}

weak_ptr<MixerFader> MixerStrip::findMixerFader()
{
	for (auto& c : children)
	{
		if (c->getName().compare("mixer-fader") == 0)
		{
			return dynamic_pointer_cast<MixerFader>(c);
		}
	}
	return {};
}

std::weak_ptr<Knob> MixerStrip::findKnob()
{
	for (auto& c : children)
	{
		if (c->getName().compare("knob") == 0)
		{
			return dynamic_pointer_cast<Knob>(c);
		}
	}
	return {};
}

std::weak_ptr<MixerTopBackground> MixerStrip::findMixerTopBackground()
{
	for (auto& c : children)
	{
		if (c->getName().compare("mixer-top-background") == 0)
		{
			return dynamic_pointer_cast<MixerTopBackground>(c);
		}
	}
	return {};
}

std::weak_ptr<MixerFaderBackground> MixerStrip::findMixerFaderBackground()
{
	for (auto& c : children)
	{
		if (c->getName().compare("mixer-fader-background") == 0)
		{
			return dynamic_pointer_cast<MixerFaderBackground>(c);
		}
	}
	return {};
}
