#include <midi/event/meta/EndOfTrack.hpp>

#include <midi/event/MidiEvent.hpp>
#include <midi/event/meta/MetaEvent.hpp>
#include <midi/util/VariableLengthInt.hpp>

using namespace mpc::midi::event::meta;

EndOfTrack::EndOfTrack(int tick, int delta)
	: MetaEvent(tick, delta, MetaEvent::END_OF_TRACK, mpc::midi::util::VariableLengthInt(0))
{
}

int EndOfTrack::getEventSize()
{
    return 3;
}

void EndOfTrack::writeToOutputStream(ostream& out) 
{
	MetaEvent::writeToOutputStream(out);
	out << 0x00;
}

int EndOfTrack::compareTo(mpc::midi::event::MidiEvent* other)
{
	if (mTick != other->getTick()) {
		return mTick < other->getTick() ? -1 : 1;
	}
	if (mDelta.getValue() != other->getDelta()) {
		return mDelta.getValue() < other->getDelta() ? 1 : -1;
	}
	if (dynamic_cast<EndOfTrack*>(other) == nullptr) {
		return 1;
	}
	return 0;
}

