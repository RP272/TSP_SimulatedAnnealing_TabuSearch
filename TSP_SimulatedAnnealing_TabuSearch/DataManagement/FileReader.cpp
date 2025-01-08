#include "FileReader.h"
#include <sstream>
#include <fstream>
#include <iostream>

ConfigurationData FileReader::readConfigurationDataFile()
{
    ConfigurationData result;
    std::ifstream configurationFile("configuration_file.txt");
    if (configurationFile.good()) {
        std::string line;
        std::getline(configurationFile, line);
        result.filename = line.substr(line.find(':') + 1);
        std::getline(configurationFile, line);
        result.stopAfterNSeconds = std::stoi(line.substr(line.find(':') + 1));
        std::getline(configurationFile, line);
        result.temperatureChangeParameter = std::stof(line.substr(line.find(':') + 1));
        std::getline(configurationFile, line);
        result.outputFilename = line.substr(line.find(':') + 1);
        std::getline(configurationFile, line);
        result.neighborGeneration = std::stoi(line.substr(line.find(':') + 1));
        std::getline(configurationFile, line);
        result.coolingScheme = std::stoi(line.substr(line.find(':') + 1));
        std::getline(configurationFile, line);
        result.numberOfMeasurements = std::stoi(line.substr(line.find(':') + 1));
        std::getline(configurationFile, line);
        result.minimalTemperature = std::stof(line.substr(line.find(':') + 1));
    }
    configurationFile.close();
    return result;
}

InputData FileReader::readInputDataFile(ConfigurationData config)
{
    InputData result = { 0 };
    std::ifstream inputFile(config.filename);
    if (inputFile.good()) {
        std::string line;
        // pobierz atrybut NAME
        std::getline(inputFile, line);
        // pobierz atrybut TYPE
        std::getline(inputFile, line);
        // pobierz atrybut COMMENT
        std::getline(inputFile, line);
        // pobierz atrybut DIMENSION
        std::getline(inputFile, line);
        result.numberOfCities = std::stoi(line.substr(line.find(':') + 1));
        // pobierz atrybut EDGE_WEIGHT_TYPE
        std::getline(inputFile, line);
        // pobierz atrybut EDGE_WEIGHT_FORMAT
        std::getline(inputFile, line);
        // pobierz atrybut EDGE_WEIGHT_SECTION
        std::getline(inputFile, line);

        // pobierz wartości wag grafu
        int rowCounter = 0, columnCounter = 0, tmp;
        result.costMatrix = new int* [result.numberOfCities];
        for (int i = 0; i < result.numberOfCities; i++) {
            result.costMatrix[i] = new int[result.numberOfCities];
        }
        
        while (std::getline(inputFile, line)) {
            std::istringstream row(line);
            while (row >> tmp) {
                result.costMatrix[rowCounter][columnCounter] = tmp;
                columnCounter++;
                if (columnCounter >= result.numberOfCities) {
                    rowCounter++;
                    columnCounter = 0;
                }
            }
        }
    }
    inputFile.close();
    return result;
};