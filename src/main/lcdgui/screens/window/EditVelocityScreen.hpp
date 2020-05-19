#pragma once
#include <lcdgui/ScreenComponent.hpp>

#include <lcdgui/screens/WithTimesAndNotes.hpp>

namespace mpc::lcdgui::screens::window {

	class EditVelocityScreen
		: public mpc::lcdgui::ScreenComponent, public mpc::lcdgui::screens::WithTimesAndNotes
	{

	public:
		void function(int i) override;
		void turnWheel(int i) override;

	public:
		EditVelocityScreen(const int& layer);

	public:
		void open() override;
		void close() override;

	private:
		int time0 = 0;
		int time1 = 0;
		int editType = 0;
		int value = 0;
		int midiNote0 = 0;
		int midiNote1 = 0;
		std::vector<std::string> editTypeNames = { "ADD VALUE", "SUB VALUE", "MULT VAL%", "SET TO VAL" };

	private:
		void setEditType(int i);
		void setValue(int i);

	private:
		void displayValue();
		void displayEditType();

	protected:
		void displayNotes() override;
		void displayTime() override;

	public:
		void update(moduru::observer::Observable* observable, nonstd::any message);

	};
}
