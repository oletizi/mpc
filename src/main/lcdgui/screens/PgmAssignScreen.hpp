#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc::lcdgui::screens {

	class PgmAssignScreen
		: public mpc::lcdgui::ScreenComponent
	{

	public:

		void function(int i) override;
		void turnWheel(int i) override;
		void openWindow() override;

		PgmAssignScreen(const int layerIndex);
		void update(moduru::observer::Observable* o, nonstd::any arg);
		void open() override;
		void close() override;

	private:
		const std::vector<std::string> soundGenerationModes{ "NORMAL", "SIMULT", "VEL SW", "DCY SW" };
		void displayPgm();
		void displaySoundName();
		void displayPadAssign();
		void displayPadNote();
		void displaySoundGenerationMode();
		void displayVeloRangeUpper();
		void displayVeloRangeLower();
		void displayOptionalNoteB();
		void displayOptionalNoteA();
		void displayNote();
		void displayPad();


	};
}
