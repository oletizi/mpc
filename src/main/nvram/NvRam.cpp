#include "NvRam.hpp"

#include <nvram/DefaultsParser.hpp>

#include <Mpc.hpp>
#include <Paths.hpp>

#include <lcdgui/screens/UserScreen.hpp>
#include <lcdgui/screens/VmpcSettingsScreen.hpp>
#include <lcdgui/screens/VmpcAutoSaveScreen.hpp>
#include <lcdgui/screens/VmpcCleanScreen.hpp>

#include <audiomidi/AudioMidiServices.hpp>

#include <hardware/Hardware.hpp>
#include <hardware/HwSlider.hpp>

#include <file/all/Defaults.hpp>

#include <file/File.hpp>
#include <file/FileUtil.hpp>

using namespace mpc::nvram;
using namespace mpc::lcdgui;
using namespace mpc::lcdgui::screens;
using namespace moduru::file;
using namespace std;

void NvRam::loadUserScreenValues(mpc::Mpc& mpc)
{
    string path = mpc::Paths::configPath() + "nvram.vmp";
    auto file = File(path, nullptr);
    
    if (!file.exists())
        return;
    
    auto defaults = DefaultsParser::AllDefaultsFromFile(mpc, file);
    auto userScreen = mpc.screens->get<UserScreen>("user");
    
    userScreen->lastBar = defaults.getBarCount() - 1;
    userScreen->bus = defaults.getBusses()[0];
    
    for (int i = 0; i < 33; i++)
    userScreen->setDeviceName(i, defaults.getDefaultDevNames()[i]);
    
    userScreen->setSequenceName(defaults.getDefaultSeqName());
    auto defTrackNames = defaults.getDefaultTrackNames();
    
    for (int i = 0; i < 64; i++)
    userScreen->setTrackName(i, defTrackNames[i]);
    
    userScreen->setDeviceNumber(defaults.getDevices()[0]);
    userScreen->setTimeSig(defaults.getTimeSigNum(), defaults.getTimeSigDen());
    userScreen->setPgm(defaults.getPgms()[0]);
    userScreen->setTempo(defaults.getTempo() / 10.0);
    userScreen->setVelo(defaults.getTrVelos()[0]);
}

void NvRam::saveUserScreenValues(mpc::Mpc& mpc)
{
    DefaultsParser dp(mpc);
    
    string fileName = mpc::Paths::configPath() + "nvram.vmp";
    
    File file(fileName, nullptr);
    
    if (!file.exists())
        file.create();
    
    auto stream = FileUtil::ofstreamw(fileName, ios::binary | ios::out);
    auto bytes = dp.getBytes();
    stream.write(&bytes[0], bytes.size());
    stream.close();
}

void NvRam::saveVmpcSettings(mpc::Mpc& mpc)
{
    auto settingsScreen = mpc.screens->get<VmpcSettingsScreen>("vmpc-settings");
    auto autoSaveScreen = mpc.screens->get<VmpcAutoSaveScreen>("vmpc-auto-save");
    auto cleanScreen = mpc.screens->get<VmpcCleanScreen>("vmpc-clean");
    
    auto audioMidiServices  = mpc.getAudioMidiServices().lock();
    string fileName = mpc::Paths::configPath() + "vmpc-specific.ini";
    
    File file(fileName, nullptr);
    
    if (!file.exists())
        file.create();
    
    auto stream = FileUtil::ofstreamw(fileName, ios::binary | ios::out);

    vector<char> bytes{
        (char) (settingsScreen->initialPadMapping),
        (char) (settingsScreen->_16LevelsEraseMode),
        (char) (autoSaveScreen->autoSaveOnExit),
        (char) (autoSaveScreen->autoLoadOnStart),
        (char) (audioMidiServices->getRecordLevel()),
        (char) (audioMidiServices->getMasterLevel()),
        (char) (mpc.getHardware().lock()->getSlider().lock()->getValue()),
        (char) (cleanScreen->showOnStartup)
    };
    
    stream.write(&bytes[0], bytes.size());
    stream.close();
}

void NvRam::loadVmpcSettings(mpc::Mpc& mpc)
{
    string path = mpc::Paths::configPath() + "vmpc-specific.ini";
    File file(path, nullptr);

    auto audioMidiServices  = mpc.getAudioMidiServices().lock();

    if (!file.exists())
    {
        audioMidiServices->setRecordLevel(DEFAULT_REC_GAIN);
        audioMidiServices->setMasterLevel(DEFAULT_MAIN_VOLUME);
        return;
    }
    
    auto settingsScreen = mpc.screens->get<VmpcSettingsScreen>("vmpc-settings");
    auto autoSaveScreen = mpc.screens->get<VmpcAutoSaveScreen>("vmpc-auto-save");
    auto cleanScreen = mpc.screens->get<VmpcCleanScreen>("vmpc-clean");
    
    auto length = file.getLength();
    vector<char> bytes(length);
    file.getData(&bytes);
    
    if (length > 0)
        settingsScreen->initialPadMapping = bytes[0];
    
    if (length > 1)
        settingsScreen->_16LevelsEraseMode = bytes[1];
    
    if (length > 2)
        autoSaveScreen->autoSaveOnExit = bytes[2];
    
    if (length > 3)
        autoSaveScreen->autoLoadOnStart = bytes[3];
    
    if (length > 4)
        audioMidiServices->setRecordLevel(bytes[4]);
    else
        audioMidiServices->setRecordLevel(DEFAULT_REC_GAIN);
    
    if (length > 5)
        audioMidiServices->setMasterLevel(bytes[5]);
    else
        audioMidiServices->setMasterLevel(DEFAULT_MAIN_VOLUME);
    
    if (length > 6)
        mpc.getHardware().lock()->getSlider().lock()->setValue(bytes[6]);
    
    if (length > 7)
        cleanScreen->setShowOnStartup(bytes[7]);
    
}
