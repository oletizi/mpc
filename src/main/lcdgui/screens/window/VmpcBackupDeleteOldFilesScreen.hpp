#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc::lcdgui::screens::window
{
	class VmpcBackupDeleteOldFilesScreen
		: public mpc::lcdgui::ScreenComponent
	{

	public:
		VmpcBackupDeleteOldFilesScreen(mpc::Mpc& mpc, const int layerIndex);

        void function(int) override;
        void turnWheel(int) override;
        void up() override;
        void down() override;
        
    private:
        int yOffset = 0;
        
	};
}
