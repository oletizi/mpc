#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc::lcdgui::screens::window
{
	class VelocityModulationScreen
		: public mpc::lcdgui::ScreenComponent
	{

	public:
		void turnWheel(int i) override;

		VelocityModulationScreen(const int layerIndex);
		
		void open() override;
		void close() override;

		void update(moduru::observer::Observable* observable, nonstd::any message);

	private:
		void displayVeloAttack();
		void displayVeloStart();
		void displayVeloLevel();
		void displayVelo();
		void displayNote();

	};
}
