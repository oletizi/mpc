#include "MidiMapping.hpp"

#include <file/File.hpp>
#include <Paths.hpp>

#include <Logger.hpp>

using namespace mpc::controls;
using namespace std;

MidiMapping::MidiMapping()
{
    importMapping();
}

vector<string> MidiMapping::getMappedLabels()
{
    vector<string> result;
    for (auto kv : labelValueMap)
        result.push_back(kv.first);
    return result;
}

void MidiMapping::exportMapping() {
	auto path = mpc::Paths::resPath() + "/midi-mapping.txt";
	moduru::file::File f(path, nullptr);
	
    if (f.exists())
		f.del();
	else
		f.create();
	
    vector<char> bytes;
	
    for (pair<string, int> x : labelValueMap)
    {
		for (char& c : x.first)
			bytes.push_back(c);
        
		bytes.push_back(' ');
		
        string value = to_string(x.second);
		
        for (char& c : value)
			bytes.push_back(c);
		
        bytes.push_back('\n');
	}
    
	f.setData(&bytes);
	f.close();
}

void MidiMapping::importMapping()
{
    auto path = mpc::Paths::resPath() + "/midi-mapping.txt";
    moduru::file::File f(path, nullptr);
    
    if (!f.exists())
    {
        MLOG("No midi-mapping.txt mapping found in vMPC/resources. Initializing default MIDI mapping...");
        initializeDefaults();
        return;
    }
    
    vector<char> bytes(f.getLength());
    
    f.getData(&bytes);
    
    string label = "";
    string value = "";
    bool parsingLabel = true;
    
    for (int i = 0; i < bytes.size(); i++)
    {
        if (bytes[i] != ' ' && parsingLabel)
        {
            label += bytes[i];
            continue;
        }
        
        if (bytes[i] == ' ')
        {
            parsingLabel = false;
            continue;
        }
        
        if (bytes[i] != ' ' && bytes[i] != '\n' && !parsingLabel)
        {
            value += bytes[i];
            continue;
        }
        
        if (bytes[i] == '\n' || i == bytes.size() - 1)
        {
            int parsedValue = -1;
            
            try
            {
                parsedValue = stoi(value);
            }
            catch (const exception& e)
            {
                // Cancel the whole function?
            }
            
            if (parsedValue != -1)
            {
                labelValueMap[label] = parsedValue;
            }
            
            label = "";
            value = "";
            parsingLabel = true;
        }
    }
}

void MidiMapping::initializeDefaults()
{
    // Note events
    // binary: 1 - 127 = push, 0 = release
    labelValueMap["left"] = 0;
    labelValueMap["right"] = 1;
    labelValueMap["up"] = 2;
    labelValueMap["down"] = 3;
    labelValueMap["rec"] = 4;
    labelValueMap["overdub"] = 5;
    labelValueMap["stop"] = 6;
    labelValueMap["play"] = 7;
    labelValueMap["play-start"] = 8;
    labelValueMap["main-screen"] = 9;
    labelValueMap["open-window"] = 10;
    labelValueMap["prev-step-event"] = 11;
    labelValueMap["next-step-event"] = 12;
    labelValueMap["go-to"] = 13;
    labelValueMap["prev-bar-start"] = 14;
    labelValueMap["next-bar-end"] = 15;
    labelValueMap["tap"] = 16;
    labelValueMap["next-seq"] = 17;
    labelValueMap["track-mute"] = 18;
    labelValueMap["full-level"] = 19;
    labelValueMap["sixteen-levels"] = 20;
    labelValueMap["f1"] = 21;
    labelValueMap["f2"] = 22;
    labelValueMap["f3"] = 23;
    labelValueMap["f4"] = 24;
    labelValueMap["f5"] = 25;
    labelValueMap["f6"] = 26;
    labelValueMap["shift"] = 27;
    labelValueMap["enter"] = 28;
    labelValueMap["undo-seq"] = 29;
    labelValueMap["erase"] = 30;
    labelValueMap["after"] = 31;
    labelValueMap["bank-a"] = 32;
    labelValueMap["bank-b"] = 33;
    labelValueMap["bank-c"] = 34;
    labelValueMap["bank-d"] = 35;
    labelValueMap["0"] = 36;
    labelValueMap["1"] = 37;
    labelValueMap["2"] = 38;
    labelValueMap["3"] = 39;
    labelValueMap["4"] = 40;
    labelValueMap["5"] = 41;
    labelValueMap["6"] = 42;
    labelValueMap["7"] = 43;
    labelValueMap["8"] = 44;
    labelValueMap["9"] = 45;

    // velocity-sensitive
    labelValueMap["pad-1"] = 46;
    labelValueMap["pad-2"] = 47;
    labelValueMap["pad-3"] = 48;
    labelValueMap["pad-4"] = 49;
    labelValueMap["pad-5"] = 50;
    labelValueMap["pad-6"] = 51;
    labelValueMap["pad-7"] = 52;
    labelValueMap["pad-8"] = 53;
    labelValueMap["pad-9"] = 54;
    labelValueMap["pad-10"] = 55;
    labelValueMap["pad-11"] = 56;
    labelValueMap["pad-12"] = 57;
    labelValueMap["pad-13"] = 58;
    labelValueMap["pad-14"] = 59;
    labelValueMap["pad-15"] = 60;
    labelValueMap["pad-16"] = 61;
    labelValueMap["datawheel-down"] = 62;
    labelValueMap["datawheel-up"] = 63;
}

int MidiMapping::getValueFromLabel(std::string label)
{
	if (labelValueMap.find(label) == labelValueMap.end())
        return -1;
	
    return labelValueMap[label];
}

std::string MidiMapping::getLabelFromValue(int value)
{
	for (std::pair<std::string, int> x : labelValueMap)
    {
		std::string label = x.first;
		
        if (value == x.second)
            return label;
	}
    
	return "";
}

void MidiMapping::setValueForLabel(const int value, std::string label)
{
    labelValueMap[label] = value;
}
