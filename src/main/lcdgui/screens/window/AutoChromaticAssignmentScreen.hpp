#pragma once
#include <lcdgui/ScreenComponent.hpp>

#include <controls/BaseSamplerControls.hpp>

namespace mpc::lcdgui::screens::window
{
	class NameScreen;
}

namespace mpc::lcdgui::screens::window
{
	class AutoChromaticAssignmentScreen
		: public mpc::lcdgui::ScreenComponent, public mpc::controls::BaseSamplerControls
	{

	public:
		void turnWheel(int i) override;

		AutoChromaticAssignmentScreen(const int layerIndex);
		void open() override;

	private:
		const std::vector<std::string> letters{ "A" , "B", "C", "D" };
		std::string newName = "";
		int sourceSoundIndex = -1;
		int originalKey = 60;
		int tune{ 0 };
		void setSourceSoundIndex(int i);
		void setOriginalKey(int i);
		void setTune(int i);

		void displayOriginalKey();
		void displaySource();
		void displayTune();
		void displayProgramName();
		void displaySnd();

	private:
		friend class mpc::lcdgui::screens::window::NameScreen;

	};
}
