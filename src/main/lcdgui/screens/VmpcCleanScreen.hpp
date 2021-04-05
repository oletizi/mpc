#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc { class Mpc; }

namespace mpc::nvram { class NvRam; }

namespace mpc::lcdgui::screens
{
class VmpcCleanScreen
: public mpc::lcdgui::ScreenComponent
{
    
public:
    void turnWheel(int i) override;
    
    VmpcCleanScreen(mpc::Mpc& mpc, const int layerIndex);
    
    void open() override;
    void function(int) override;
    void openWindow() override;
        
private:
    bool showOnStartup = true;
    bool oldFilesFound = false;
    
    void setShowOnStartup(bool);
    void initOldFilesFound();
    
    void displayShowOnStartup();
    
    friend class mpc::Mpc;
    friend class mpc::nvram::NvRam;
    
};
}
