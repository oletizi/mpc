#include <controls/mixer/SelectDrumMixerControls.hpp>

#include <ui/sampler/SamplerGui.hpp>
#include <ui/sampler/MixerGui.hpp>

using namespace mpc::controls::mixer;
using namespace std;

SelectDrumMixerControls::SelectDrumMixerControls() 
	: AbstractMixerControls()
{
}

void SelectDrumMixerControls::function(int i)
{
	init();
	if (i < 4) {
		samplerGui->setSelectedDrum(i);
		samplerGui->setPrevScreenName(csn);
		ls.lock()->openScreen("mixer");
	}
}

SelectDrumMixerControls::~SelectDrumMixerControls() {
}
