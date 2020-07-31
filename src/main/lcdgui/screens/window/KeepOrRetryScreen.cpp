#include "KeepOrRetryScreen.hpp"

#include <lcdgui/screens/window/NameScreen.hpp>

#include <sampler/NoteParameters.hpp>

using namespace mpc::lcdgui::screens::window;
using namespace std;

KeepOrRetryScreen::KeepOrRetryScreen(mpc::Mpc& mpc, const int layerIndex) 
	: ScreenComponent(mpc, "keep-or-retry", layerIndex)
{
}

void KeepOrRetryScreen::open()
{
	init();
	displayNameForNewSound();
	displayAssignToNote();
	mpc.addObserver(this); // Subscribe to "padandnote" message
}

void KeepOrRetryScreen::close()
{
	mpc.deleteObserver(this);
}

void KeepOrRetryScreen::mainScreen() {
	
	sampler.lock()->deleteSound(dynamic_pointer_cast<mpc::sampler::Sound>(sampler.lock()->getPreviewSound().lock()));
	baseControls->mainScreen();
}

void KeepOrRetryScreen::function(int i)
{
	init();
	
	switch (i) {
	case 1:
		sampler.lock()->deleteSound(dynamic_pointer_cast<mpc::sampler::Sound>(sampler.lock()->getPreviewSound().lock()));
		ls.lock()->openScreen("sample");
		break;
	case 3 :
		if (mpc.getControls().lock()->isF4Pressed())
		{
			return;
		}
		mpc.getControls().lock()->setF4Pressed(true);
		sampler.lock()->playPreviewSample(0, sampler.lock()->getPreviewSound().lock()->getLastFrameIndex(), 0, 2);
		break;
	case 4:
		sampler.lock()->getLastNp(program.lock().get())->setSoundNumber(sampler.lock()->getSoundCount() - 1);
		sampler.lock()->setSoundIndex(sampler.lock()->getSoundCount() - 1);
		ls.lock()->openScreen("sample");
		break;
	}
}

void KeepOrRetryScreen::turnWheel(int i)
{
    init();
	auto nameScreen = dynamic_pointer_cast<NameScreen>(mpc.screens->getScreenComponent("name"));
	nameScreen->setName(dynamic_pointer_cast<mpc::sampler::Sound>(sampler.lock()->getPreviewSound().lock())->getName());
	nameScreen->parameterName = param;
    ls.lock()->openScreen("name");
}

void KeepOrRetryScreen::displayNameForNewSound()
{
	if (!sampler.lock()->getSound().lock())
	{
		return;
	}
	findField("name-for-new-sound").lock()->setText(dynamic_pointer_cast<mpc::sampler::Sound>(sampler.lock()->getPreviewSound().lock())->getName());
}

void KeepOrRetryScreen::displayAssignToNote()
{
	auto note = to_string(sampler.lock()->getLastPad(program.lock().get())->getNote());
	auto pad = sampler.lock()->getPadName(sampler.lock()->getLastPad(program.lock().get())->getNumber());
	findField("assign-to-note").lock()->setText(note + "/" + pad);
}

void KeepOrRetryScreen::update(moduru::observer::Observable* o, nonstd::any arg)
{
	string s = nonstd::any_cast<string>(arg);

	if (s.compare("padandnote") == 0)
	{
		displayAssignToNote();
	}
}
