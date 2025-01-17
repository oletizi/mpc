#include "VmpcDirectToDiskRecorderScreen.hpp"

#include <audiomidi/AudioMidiServices.hpp>
#include <audiomidi/DirectToDiskSettings.hpp>

#include <sequencer/Song.hpp>
#include <sequencer/SeqUtil.hpp>
#include <audio/server/NonRealTimeAudioServer.hpp>

#include <lcdgui/Screens.hpp>
#include <lcdgui/screens/SongScreen.hpp>
#include <lcdgui/screens/window/NameScreen.hpp>

#include <Util.hpp>

using namespace mpc;
using namespace mpc::audiomidi;
using namespace mpc::lcdgui::screens;
using namespace mpc::lcdgui::screens::window;
using namespace mpc::lcdgui;
using namespace mpc::sequencer;
using namespace moduru::lang;
using namespace std;

VmpcDirectToDiskRecorderScreen::VmpcDirectToDiskRecorderScreen(mpc::Mpc& mpc, const int layerIndex)
	: ScreenComponent(mpc, "vmpc-direct-to-disk-recorder", layerIndex)
{
}

void VmpcDirectToDiskRecorderScreen::open()
{
	setSq(sequencer.lock()->getActiveSequenceIndex());
	displayRecord();
	displaySong();
	displayTime();
	displayOffline();
	displaySplitLR();
	displayRate();
}

void VmpcDirectToDiskRecorderScreen::turnWheel(int i)
{
	init();

	auto seq = mpc.getSequencer().lock()->getSequence(sq).lock().get();

	checkAllTimes(mpc, i, seq);

	if (param.compare("rate") == 0)
	{
		setSampleRate(sampleRate + i);
	}
	if (param.compare("record") == 0)
	{
		setRecord(record + i);
	}
	else if (param.compare("sq") == 0)
	{
		setSq(sq + i);
	}
	else if (param.compare("song") == 0)
	{
		setSong(song + i);
	}
	else if (param.compare("split-lr") == 0)
	{
		setSplitLR(i > 0);
	}
	else if (param.compare("offline") == 0)
	{
		setOffline(i > 0);
	}
}

void VmpcDirectToDiskRecorderScreen::function(int i)
{
	init();

	switch (i)
	{
	case 3:
		openScreen("sequencer");
		break;
	case 4:
	{
		auto seq = sq;
		vector<int> rates{ 44100, 48000, 88200 };
		auto rate = rates[sampleRate];

		if (!offline)
			rate = mpc.getAudioMidiServices().lock()->getAudioServer()->getSampleRate();

		auto split = false;
		auto sequence = sequencer.lock()->getSequence(seq).lock();

		switch (record)
		{
		case 0:
		{
			openScreen("sequencer");
			sequence->setLoopEnabled(false);
			auto lengthInFrames = SeqUtil::sequenceFrameLength(sequence.get(), 0, sequence->getLastTick(), rate);
			auto settings = make_unique<DirectToDiskSettings>(lengthInFrames, outputFolder, split, rate);
			
			if (!mpc.getAudioMidiServices().lock()->prepareBouncing(settings.get()))
				openScreen("vmpc-file-in-use");
			else			
				sequencer.lock()->playFromStart();

			break;
		}
		case 1:
		{
			openScreen("sequencer");
			auto lengthInFrames = SeqUtil::loopFrameLength(sequence.get(), rate);
			auto settings = make_unique<DirectToDiskSettings>(lengthInFrames, outputFolder, split, rate);
			sequence->setLoopEnabled(false);
			sequencer.lock()->move(sequence->getLoopStart());

			if (!mpc.getAudioMidiServices().lock()->prepareBouncing(settings.get()))
				openScreen("vmpc-file-in-use");
			else
				sequencer.lock()->play();

			break;
		}
		case 2:
		{
			openScreen("sequencer");
			auto lengthInFrames = SeqUtil::sequenceFrameLength(sequence.get(), time0, time1, rate);
			auto settings = make_unique<DirectToDiskSettings>(lengthInFrames, outputFolder, split, rate);
			sequence->setLoopEnabled(false);
			sequencer.lock()->move(time0);
			
			if (!mpc.getAudioMidiServices().lock()->prepareBouncing(settings.get()))
				openScreen("vmpc-file-in-use");
			else
				sequencer.lock()->play();
			
			break;
		}
		case 3:
		{
			auto mpcSong = sequencer.lock()->getSong(song).lock();

			if (!mpcSong->isUsed())
				return;

			auto lengthInFrames = SeqUtil::songFrameLength(mpcSong.get(), sequencer.lock().get(), rate);
			auto settings = make_unique<DirectToDiskSettings>(lengthInFrames, outputFolder, split, rate);

			openScreen("song");

			sequencer.lock()->setSongModeEnabled(true);

			auto songScreen = mpc.screens->get<SongScreen>("song");
			songScreen->setLoop(false);

			if (!mpc.getAudioMidiServices().lock()->prepareBouncing(settings.get()))
				openScreen("vmpc-file-in-use");
			else
				sequencer.lock()->playFromStart();

			break;
		}
		case 4:
			openScreen("vmpc-record-jam");
			break;
		}
	}
	}
}

void VmpcDirectToDiskRecorderScreen::setSampleRate(int rate)
{
	if (rate < 0 || rate > 2)
		return;

	sampleRate = rate;
	displayRate();
}

void VmpcDirectToDiskRecorderScreen::setRecord(int i)
{
	if (i < 0 || i > 4)
		return;

	record = i;

	displayRecord();
	displaySq();
	displaySong();
	displayTime();
	displayOffline();
}

void VmpcDirectToDiskRecorderScreen::setSq(int i)
{
	if (i < 0 || i > 99)
		return;

	sq = i;
	
	setTime0(0);
	
	auto s = mpc.getSequencer().lock()->getSequence(sq).lock();
	
	if (s->isUsed())
		setTime1(s->getLastTick());
	else
		setTime1(0);

	displaySq();
}

void VmpcDirectToDiskRecorderScreen::setSong(int i)
{
	if (i < 0 || i > 4)
		return;
	
	song = i;
	displaySong();
}

void VmpcDirectToDiskRecorderScreen::setOffline(bool b)
{
	offline = b;
	displayOffline();
	displayRate();
}

void VmpcDirectToDiskRecorderScreen::setSplitLR(bool b)
{
	splitLR = b;
	displaySplitLR();
}

void VmpcDirectToDiskRecorderScreen::displayRate()
{
	findField("rate").lock()->Hide(!offline);
	findLabel("rate").lock()->Hide(!offline);
	
	if (!offline)
		return;

	vector<string> rates{ "44.1", "48.0", "88.2" };
	string rate = Util::replaceDotWithSmallSpaceDot(rates[sampleRate]);
	findField("rate").lock()->setText(rate);
}

void VmpcDirectToDiskRecorderScreen::displaySong()
{
	findField("song").lock()->Hide(record != 3);
	findLabel("song").lock()->Hide(record != 3);

	if (record != 3)
		return;

	findField("song").lock()->setText(StrUtil::padLeft(to_string(song + 1), "0", 2) + "-" + mpc.getSequencer().lock()->getSong(song).lock()->getName());
}

void VmpcDirectToDiskRecorderScreen::displayOffline()
{
	findField("offline").lock()->Hide(record == 4);
	findLabel("offline").lock()->Hide(record == 4);
	
	if (record != 4)
		findField("offline").lock()->setText(offline ? "YES" : "NO");
}

void VmpcDirectToDiskRecorderScreen::displaySplitLR()
{
	findField("split-lr").lock()->setText(splitLR ? "YES" : "NO");
}

void VmpcDirectToDiskRecorderScreen::displayRecord()
{
	findField("record").lock()->setText(recordNames[record]);
}

void VmpcDirectToDiskRecorderScreen::displaySq()
{
	auto visible = (record >= 0 && record <= 2);
	
	findField("sq").lock()->Hide(!visible);
	findLabel("sq").lock()->Hide(!visible);

	if (!visible)
		return;

	findField("sq").lock()->setText(StrUtil::padLeft(to_string(sq + 1), "0", 2) + "-" + mpc.getSequencer().lock()->getSequence(sq).lock()->getName());
}

void VmpcDirectToDiskRecorderScreen::displayTime()
{
	auto invisible = record != 2;

	for (int i = 0; i < 6; i++)
	{
		findField("time" + to_string(i)).lock()->Hide(invisible);
		findLabel("time" + to_string(i)).lock()->Hide(invisible);
	}

	if (invisible)
		return;

	auto sequence = mpc.getSequencer().lock()->getSequence(sq).lock();

	findField("time0").lock()->setTextPadded(SeqUtil::getBar(sequence.get(), time0 ) + 1, "0");
	findField("time1").lock()->setTextPadded(SeqUtil::getBeat(sequence.get(), time0) + 1, "0");
	findField("time2").lock()->setTextPadded(SeqUtil::getClock(sequence.get(), time0), "0");
	findField("time3").lock()->setTextPadded(SeqUtil::getBar(sequence.get(), time1) + 1, "0");
	findField("time4").lock()->setTextPadded(SeqUtil::getBeat(sequence.get(), time1) + 1, "0");
	findField("time5").lock()->setTextPadded(SeqUtil::getClock(sequence.get(), time1), "0");
}

bool VmpcDirectToDiskRecorderScreen::isOffline()
{
	return offline;
}

int VmpcDirectToDiskRecorderScreen::getSampleRate()
{
	return sampleRate;
}

int VmpcDirectToDiskRecorderScreen::getRecord()
{
	return record;
}
