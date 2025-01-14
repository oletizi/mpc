#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc { class Mpc; }
namespace mpc::disk { class ProgramLoader; class AbstractDisk; }

namespace mpc::lcdgui::screens::window
{
	class LoadAProgramScreen
		: public mpc::lcdgui::ScreenComponent
	{

	public:
		void turnWheel(int i) override;
		void function(int i) override;

		LoadAProgramScreen(mpc::Mpc& mpc, const int layerIndex);
		void open() override;

	private:
		bool loadReplaceSound = false;
		bool clearProgramWhenLoading = false;

		void displayLoadReplaceSound();

        friend class mpc::Mpc;
        friend class mpc::disk::AbstractDisk;
		friend class mpc::disk::ProgramLoader;

	};
}
