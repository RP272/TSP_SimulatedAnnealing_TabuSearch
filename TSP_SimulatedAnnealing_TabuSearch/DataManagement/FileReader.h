#ifndef FILEREADER_H
#define FILEREADER_H
#include "InputData.h"
#include "ConfigurationData.h"

class FileReader
{
public:
    static ConfigurationData readConfigurationDataFile();
    static InputData readInputDataFile(ConfigurationData config);
};

#endif