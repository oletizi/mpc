#pragma once
#include <observer/Observer.hpp>
#include <sequencer/TimeSignature.hpp>

#include <memory>
#include <vector>
#include <string>

namespace mpc::lcdgui {
	class Field;
	class Label;
}

namespace mpc::ui {
	class UserDefaults;
}

namespace mpc::ui::sequencer {

	class UserObserver
		: public moduru::observer::Observer
	{

	private:
		mpc::ui::UserDefaults& ud;
		std::vector<std::string> busNames;;
		;
		mpc::sequencer::TimeSignature& timeSig;
		std::weak_ptr<mpc::lcdgui::Field> tempoField;
		std::weak_ptr<mpc::lcdgui::Field> loopField;
		std::weak_ptr<mpc::lcdgui::Field> tsigField;
		std::weak_ptr<mpc::lcdgui::Field> barsField;
		std::weak_ptr<mpc::lcdgui::Field> pgmField;
		std::weak_ptr<mpc::lcdgui::Field> recordingModeField;
		std::weak_ptr<mpc::lcdgui::Field> busField;
		std::weak_ptr<mpc::lcdgui::Field> deviceNumberField;
		std::weak_ptr<mpc::lcdgui::Label> deviceNameLabel;
		std::weak_ptr<mpc::lcdgui::Field> veloField;


	public:
		void update(moduru::observer::Observable* o, nonstd::any arg) override;

	public:
		UserObserver();
		~UserObserver();

	};
}
