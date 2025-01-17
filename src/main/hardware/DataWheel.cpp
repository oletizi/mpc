#include "DataWheel.hpp"

#include <Mpc.hpp>
#include <lcdgui/ScreenComponent.hpp>
#include <disk/AbstractDisk.hpp>

using namespace mpc::hardware;
using namespace std;

DataWheel::DataWheel(mpc::Mpc& mpc)
	: mpc(mpc)
{
}

void DataWheel::turn(int increment)
{
	auto controls = mpc.getActiveControls().lock();
	
    if (controls) controls->turnWheel(increment);

	notifyObservers(increment);
}
