#pragma once
#include <disk/AbstractDisk.hpp>
#include <disk/Volume.hpp>

#include <file/File.hpp>

#include <memory>
#include <string>
#include <vector>

namespace mpc::disk {
class MpcFile;

class StdDisk
: public AbstractDisk
{
    
public:
    StdDisk(mpc::Mpc&);
    std::shared_ptr<MpcFile> getDir();

private:
    std::shared_ptr<mpc::disk::MpcFile> root;
    Volume volume;
    std::vector<std::string> path;
    void initParentFiles();
    std::shared_ptr<MpcFile> getParentDir();
    void renameFilesToAkai();
    
public:
    void initFiles() override;
    std::string getDirectoryName() override;
    bool moveBack() override;
    bool moveForward(const std::string& directoryName) override;
    void close() override;
    void flush() override;
    bool deleteAllFiles(int dwGuiDelete) override;
    bool newFolder(const std::string& newDirName) override;
    std::shared_ptr<MpcFile> newFile(const std::string& newFileName) override;
    std::string getAbsolutePath() override;
    std::string getTypeShortName() override;
    std::string getModeShortName() override;
    uint64_t getTotalSize() override;
    std::string getVolumeLabel() override;
    Volume& getVolume() override { return volume; }
    void initRoot() override { root = volume.getRoot(); }
    
protected:
    int getPathDepth() override;
        
};
}
