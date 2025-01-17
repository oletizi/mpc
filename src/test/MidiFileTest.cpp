#include <catch2/catch.hpp>

#include <Mpc.hpp>

#include <sequencer/Sequencer.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>

#include <midi/MidiFile.hpp>
#include <file/mid/MidiReader.hpp>
#include <file/mid/MidiWriter.hpp>

#include <string>
#include <vector>

using namespace mpc;
using namespace mpc::disk;
using namespace mpc::midi;
using namespace mpc::file::mid;

SCENARIO("A MidiFile can be written", "[file]") {

	GIVEN("An Mpc with a Sequence") {

		mpc::Mpc mpc;
		mpc.init(44100, 1, 1);
		auto sequencer = mpc.getSequencer().lock();
		auto sequence = sequencer->getSequence(0).lock();
		sequence->init(1);
		auto track = sequence->getTrack(0).lock();
		track->setUsed(true);

		auto noteEvent = track->addNoteEvent(0, 37).lock();
		noteEvent->setDuration(10);
		noteEvent->setVelocity(127);

		MidiWriter midiWriter(sequence.get());
        auto ostream = std::make_shared<std::ostringstream>();
        midiWriter.writeToOStream(ostream);
		
        sequence->init(1);
        sequence->getTrack(0).lock()->removeEvents();
        REQUIRE(sequence->getTrack(0).lock()->getEvents().size() == 0);
        
        auto istream = std::make_shared<std::istringstream>(ostream->str());
        MidiReader midiReader(istream, sequence);
        midiReader.parseSequence(mpc);
        
        REQUIRE(sequence->getTrack(0).lock()->getEvents().size() == 1);

    }
}
