#pragma once
#include <disk/SoundSaver.hpp>
#include <disk/AllLoader.hpp>

#include <vector>
#include <string>

#include <mpc_types.hpp>

namespace mpc { class Mpc; }

namespace mpc::lcdgui::screens::window {
class LoadASequenceScreen;
class LoadAProgramScreen;
class LoadApsFileScreen;
class Mpc2000XlAllFileScreen;
}

namespace mpc::sequencer { class Sequence; }

namespace mpc::sampler { class Program; class Sound; }

namespace mpc::disk {

class MpcFile;
class Volume;

class AbstractDisk
{
protected:
    AbstractDisk(mpc::Mpc&);

    mpc::Mpc& mpc;
    const std::vector<std::string> extensions{ "", "SND", "PGM", "APS", "MID", "ALL", "WAV", "SEQ", "SET" };
    std::vector<std::shared_ptr<MpcFile>> files;
    std::vector<std::shared_ptr<MpcFile>> allFiles;
    std::vector<std::shared_ptr<MpcFile>> parentFiles;

    virtual int getPathDepth() = 0;
    virtual std::shared_ptr<MpcFile> newFile(const std::string& name) = 0;

public:
    bool deleteSelectedFile();
    
    std::vector<std::string> getFileNames();
    std::string getFileName(int i);
    std::vector<std::string> getParentFileNames();
    
    std::shared_ptr<MpcFile> getFile(int i);
    std::shared_ptr<MpcFile> getFile(const std::string& fileName);
    std::vector<std::shared_ptr<MpcFile>>& getAllFiles();
    std::shared_ptr<MpcFile> getParentFile(int i);
    
    void writeSnd(std::shared_ptr<mpc::sampler::Sound>, std::string fileName);
    void writeWav(std::shared_ptr<mpc::sampler::Sound>, std::string fileName);
    void writeMid(std::shared_ptr<mpc::sequencer::Sequence>, std::string fileName);
    void writePgm(std::shared_ptr<mpc::sampler::Program>, const std::string& fileName);
    void writeAps(const std::string& fileName);
    void writeAll(const std::string& fileName);
    
    bool checkExists(std::string fileName);
    bool deleteRecursive(std::weak_ptr<MpcFile>);
    
    bool isRoot();
    
    virtual void flush() = 0;
    virtual void close() = 0;
    virtual void initFiles() = 0;
    virtual std::string getDirectoryName() = 0;
    virtual bool moveBack() = 0;
    virtual bool moveForward(const std::string& directoryName) = 0;
    virtual bool deleteAllFiles(int dwGuiDelete) = 0;
    virtual bool newFolder(const std::string& newDirName) = 0;
    virtual std::string getAbsolutePath() = 0;
    virtual std::string getTypeShortName() = 0;
    virtual std::string getModeShortName() = 0;
    virtual uint64_t getTotalSize() = 0;
    virtual std::string getVolumeLabel() = 0;
    virtual Volume& getVolume() = 0;
    virtual void initRoot() = 0;

private:
    std::function<void(mpc_io_error e)> errorFunc;
    std::unique_ptr<SoundSaver> soundSaver;
    std::unique_ptr<AllLoader> allLoader;

    file_or_error newFile2(const std::string& name);
    file_or_error writeWav2(std::shared_ptr<mpc::sampler::Sound>, std::shared_ptr<MpcFile>);
    file_or_error writeSnd2(std::shared_ptr<mpc::sampler::Sound>, std::shared_ptr<MpcFile>);
    file_or_error writeMid2(std::shared_ptr<mpc::sequencer::Sequence>, std::shared_ptr<MpcFile>);
    file_or_error writePgm2(std::shared_ptr<mpc::sampler::Program>, std::shared_ptr<MpcFile>);
    file_or_error writeAps2(std::shared_ptr<MpcFile>);
    file_or_error writeAll2(std::shared_ptr<MpcFile>);

    friend class SoundLoader; // Temporary access to readWav2 and readSnd2 until a readWav/readSnd/etc interface is exposed
    friend class mpc::lcdgui::screens::window::LoadASequenceScreen;
    friend class mpc::lcdgui::screens::window::LoadAProgramScreen;
    friend class mpc::lcdgui::screens::window::LoadApsFileScreen;
    friend class mpc::lcdgui::screens::window::Mpc2000XlAllFileScreen;
    
    sound_or_error readWav2(std::shared_ptr<MpcFile>);
    sound_or_error readSnd2(std::shared_ptr<MpcFile>);
    sequence_or_error readMid2(std::shared_ptr<MpcFile>);
    void readPgm2(std::shared_ptr<MpcFile>);
    void readAps2(std::shared_ptr<MpcFile>, std::function<void()> on_success);
    void readAll2(std::shared_ptr<MpcFile>, std::function<void()> on_success);
    sequences_or_error readSequencesFromAll2(std::shared_ptr<MpcFile>);

};
}
