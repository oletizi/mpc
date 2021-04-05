#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc::lcdgui::screens::window
{
class VmpcBackupDeleteOldFilesScreen
: public mpc::lcdgui::ScreenComponent
{
    
public:
    VmpcBackupDeleteOldFilesScreen(mpc::Mpc& mpc, const int layerIndex);
    
    void open() override;
    void function(int) override;
    void turnWheel(int) override;
    void up() override;
    void down() override;
    
private:
    std::vector<std::string> actionNames {
        "Backup user data             :",
        "Import keyboard mapping      :",
        "Import NVRAM (USER defaults) :",
        "Import VMPC specific settings:",
        "Delete old applications      :",
        "Delete Users/you/vMPC folder :"
    };

    std::vector<bool> actionValues {
      true, true, true, true, false, false
    };
    
    std::vector<bool> actionAvailability {
      false, false, false, false, false, false
    };
    
    int yOffset = 0;

    void displayActions();
    
    void determineAvailableActions();
    
};
}
