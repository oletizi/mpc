#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>

namespace mpc { class Mpc; }

namespace mpc::controls {
class KbMapping;
class MidiMapping;
class KeyEventHandler;
class BaseControls;
class GlobalReleaseControls;
}

namespace mpc::controls {
class Controls
{
public:
    Controls(mpc::Mpc&);
    ~Controls();
    
    void setErasePressed(bool);
    void setRecPressed(bool);
    void setOverDubPressed(bool);
    void setTapPressed(bool);
    void setGoToPressed(bool);
    void setShiftPressed(bool);
    void setF3Pressed(bool);
    void setF4Pressed(bool);
    void setF5Pressed(bool);
    void setF6Pressed(bool);
    void setNoteRepeatLocked(bool);
    
    bool isErasePressed();
    bool isRecPressed();
    bool isOverDubPressed();
    bool isTapPressed();
    bool isNoteRepeatLocked();
    bool isGoToPressed();
    bool isShiftPressed();
    bool isF3Pressed();
    bool isF4Pressed();
    bool isF5Pressed();
    bool isF6Pressed();
    
    std::set<int>* getPressedPads();
    std::vector<int>* getPressedPadVelos();
    
    void releaseAll();
    std::weak_ptr<KeyEventHandler> getKeyEventHandler();
    mpc::controls::BaseControls* getControls(std::string s);
    mpc::controls::GlobalReleaseControls* getReleaseControls();
    
    void setCtrlPressed(bool);
    void setAltPressed(bool);
    bool isCtrlPressed();
    bool isAltPressed();
    std::weak_ptr<KbMapping> getKbMapping();
    std::weak_ptr<MidiMapping> getMidiMapping();
    
private:
    std::shared_ptr<KbMapping> kbMapping;
    std::shared_ptr<MidiMapping> midiMapping;
    bool ctrlPressed = false;
    bool altPressed = false;
    bool shiftPressed = false;
    bool recPressed = false;
    bool overDubPressed = false;
    bool tapPressed = false;
    bool noteRepeatLocked = false;
    bool goToPressed = false;
    bool erasePressed = false;
    bool f3Pressed = false;
    bool f4Pressed = false;
    bool f5Pressed = false;
    bool f6Pressed = false;
    std::set<int> pressedPads;
    std::vector<int> pressedPadVelos;
    std::shared_ptr<KeyEventHandler> keyEventHandler;
    std::unordered_map<std::string, controls::BaseControls*> controls;
    
};
}
