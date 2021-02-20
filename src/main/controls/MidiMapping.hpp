#pragma once
#include <cstdint>
#include <vector>

#include <unordered_map>
#include <vector>
#include <string>

namespace mpc::controls
{
class MidiMapping
{
    
public:
    void initializeDefaults();
    int getValueFromLabel(std::string label);
    std::string getLabelFromValue(const int value);
    void setValueForLabel(const int value, std::string label);
    std::vector<std::string> getMappedLabels();
    
    void exportMapping();
    void importMapping();
    
    MidiMapping();
    
private:
    std::unordered_map<std::string, int> labelValueMap;
    
};
}
