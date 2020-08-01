#include "Field.hpp"

#include "Label.hpp"

#include <Mpc.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/TwoDots.hpp>

#include <lang/StrUtil.hpp>

#include <file/File.hpp>

#ifdef __linux__
#include <climits>
#endif // __linux__

#include <stdexcept>

using namespace mpc::lcdgui;
using namespace moduru::lang;
using namespace std;

Field::Field(mpc::Mpc& mpc, const string& name, int x, int y, int width)
	: TextComp(mpc, name), mpc(mpc)
{
	this->name = name;
	
	if (width == 1)
	{
		// We use width 0 as an indicator of wanting to be completely invisible, like dummy fields
		width = 0;
	}

	setSize(width, 9);
	setLocation(x - 1, y - 1);
	preDrawClearRect.Clear();
}

void Field::setNextFocus(const std::string& nextFocus)
{
	this->nextFocus = nextFocus;
}

void Field::Hide(bool b)
{
	if (b && focus)
	{
		mpc.getLayeredScreen().lock()->setFocus(nextFocus);
	}

	Component::Hide(b);
}

void Field::Draw(std::vector<std::vector<bool>>* pixels)
{
	if (shouldNotDraw(pixels))
	{
		return;
	}
	
	auto r = getRect();

	for (int i = r.L; i < r.R; i++)
	{
		if (i < 0)
		{
			continue;
		}

		for (int j = r.T; j < r.B; j++)
		{
			if (j < 0)
			{
				continue;
			}
			(*pixels)[i][j] = inverted;
		}
	}
	TextComp::Draw(pixels);
}

void Field::takeFocus(string prev)
{
	auto layeredScreen = mpc.getLayeredScreen().lock();
	csn = layeredScreen->getCurrentScreenName();
	focus = true;
	inverted = true;
	SetDirty();
}

void Field::loseFocus(string next)
{
	focus = false;
	inverted = false;
	SetDirty();
}


void Field::setSplit(bool b)
{
	if (split == b)
		return;
	
	split = b;
	
	if (split)
	{
		activeSplit = text.length() - 1;

		for (int i = 0; i < text.length(); i++) {
			auto field = parent->addChild(make_shared<Field>(mpc, "split" + to_string(i), x + (i * FONT_WIDTH) + 1, y + 1, 7)).lock();
			dynamic_pointer_cast<Field>(field)->setText(text.substr(i, i + 1));
		}

		oldText = text;
		setText("");
		redrawSplit();
	}
	else
	{
		for (int i = 0; i < oldText.length(); i++)
			parent->removeChild(parent->findChild<Field>("split" + to_string(i)));

		activeSplit = 0;
		setTextPadded(oldText);
	}
}

void Field::redrawSplit()
{
	for (int i = 0; i < oldText.length(); i++) {
		parent->findChild<Field>("split" + to_string(i)).lock()->setInverted(i < activeSplit);
	}
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
	if (i < 1 || i + 1 > oldText.size())
	{
		return false;
	}

	activeSplit = i;
	redrawSplit();
	return true;
}

bool Field::enableTypeMode()
{
	if (typeModeEnabled)
	{
		return false;
	}

    typeModeEnabled = true;
	oldText = text;
	setText("");
    return true;
}

int Field::enter()
{
	auto value = INT_MAX;

	if (!typeModeEnabled)
	{
		return value;
	}

    typeModeEnabled = false;
    
	try
	{
		value = stoi(getText());
	}
	catch (std::invalid_argument& e)
	{
		printf("Field.enter ERROR: %s", e.what());
        return value;
    }
    setText(oldText);
    return value;
}

void Field::type(int i)
{
	auto textCopy = StrUtil::replaceAll(getText(), ' ', "");
	
	if (textCopy.length() == floor(w / FONT_WIDTH))
	{
		textCopy = "";
	}

	auto newText = textCopy.append(to_string(i));
	setTextPadded(newText.c_str());
}

bool Field::isTypeModeEnabled()
{
    return typeModeEnabled;
}

void Field::disableTypeMode()
{
	if (!typeModeEnabled)
		return;

    typeModeEnabled = false;
    setText(oldText.c_str());
}

void Field::setFocusable(bool b)
{
	focusable = b;
}

bool Field::isFocusable()
{
	return focusable;
}

bool Field::hasFocus()
{
	return focus;
}
