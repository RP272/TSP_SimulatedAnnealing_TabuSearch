#ifndef FILEREADER_HPP
#define FILEREADER_HPP
#include "InputData.hpp"
#include "ConfigurationData.hpp"

class FileReader
{
public:
    static ConfigurationData readConfigurationDataFile();
    static InputData readInputDataFile(ConfigurationData config);
};

#endif